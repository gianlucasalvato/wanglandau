#!/bin/bash

CMD="`pwd`/wl"
workdir="`pwd`/work"

runs=1
max_threads=6
sleep_time=20

if [ ! -f "$CMD" ]; then
	./build.sh
	EXIT_CODE=$?
	[[ "$EXIT_CODE" -ne 0 ]] && echo "Could not start program!" && exit $EXIT_CODE
fi

edges=( 64 )
f_initial=`bc -l <<< "e(1)"`
f_threshold=`bc -l <<< "1 + 10^(-8)"`
f_shrink=0.5
flatness_threshold=0.8

args_base="--mod-factor $f_initial"
args_base+=" --mod-factor-threshold $f_threshold"
args_base+=" --shrink-factor $f_shrink"
args_base+=" --flatness-threshold $flatness_threshold"

start () {
	local start_time=`date +%s`

	$CMD $1 >> /dev/null
#	echo "$CMD $1"

	local end_time=`date +%s`
	local dt=`bc <<< "$end_time - $start_time"`
	local dm=`bc <<< "$dt/60"`
	local ds=`bc <<< "$dt-60*$dm"`

	local Tdt=`bc <<< "$end_time - $start_exec"`
	local Tdh=`bc <<< "$Tdt/3600"`
	local Tdm=`bc <<< "($Tdt-3600*$Tdh)/60"`
	local Tds=`bc <<< "$Tdt-60*($Tdm+60*$Tdh)"`

	echo
	echo -e "(`date '+%H:%M:%S'`) Run finished ($dm:$ds - Total: $Tdh:$Tdm:$Tds)."
}

echo "Starting data collection..."
start_exec=$(date +%s)

[[ ! -d "$workdir" ]] && mkdir -p "$workdir"

threads=0
fail=0

for edge in ${edges[@]}; do
	edgedir="$workdir/$edge" && mkdir -p "$edgedir"
	cd "$edgedir" >> /dev/null
	
	args_run="$args_base --edge-length $edge"
		
	for run in $(seq 1 $runs); do
		echo -e "Starting run $run with l=$edge"
	
		rundir="$edgedir/$run" && mkdir -p "$rundir"
		cd "$rundir" >> /dev/null
	
		seed=`od -N 4 -t uL -An /dev/urandom | tr -d " "`
		args="$args_run --seed $seed"

		start "$args" &
		pids="$pids $!"
		
		echo -e " (pid=$!)\n"
		
		threads=$((threads + 1))
		
		cd "$edgedir" >> /dev/null	
		
		if [ "$threads" -ge "$max_threads" ]; then
			echo "Waiting for jobs to finish..."
			for pid in $pids; do wait $pid || let "fail+=1"; done
			threads=0 && sleep $sleep_time
		fi
	done
	cd "$workdir" >> /dev/null
done

for pid in $pids; do wait $pid || let "fail+=1"; done

end_exec=`date +%s`
dt=`bc <<< "$end_exec - $start_exec"`
dh=`bc <<< "$dt/3600"`
dm=`bc <<< "($dt-3600*$dh)/60"`
ds=`bc <<< "$dt-60*($dm+60*$dh)"`

echo "(`date '+%H:%M:%S'`) Script completed."
echo "Total execution time: $dh h, $dm m, $ds s"
echo
echo "$fail job(s) failed to complete."

exit $fail
