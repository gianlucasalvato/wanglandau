#!/bin/bash

CMD="`pwd`/bin/wl"
workdir="`pwd`/work"

if [ ! -f $CMD ]; then
	echo "Cannot find executable. This script must run in the project root directory!"
	exit 0
fi

[ "$1" == "--dry" ] && DRY=1

runs=1
max_threads=6
sleep_time=20

if [ ! -f "$CMD" ]; then
	./build.sh
	EXIT_CODE=$?
	[[ "$EXIT_CODE" -ne 0 ]] && echo "Could not build program!" && exit $EXIT_CODE
fi

edges=( 8 )
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

	[[ $DRY == 1 ]] && echo "$CMD $1" || $CMD $1 >> /dev/null

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

# Need a check because process ids are not guaranteed to be unique over a long time.
# If the pid did become available and the system did reassign it to another process
# before the execution of this code, we do NOT kill that process (unless its another 'wl'
# run of course, which may however seem unlikely).
kill_wl () {
	for pid in $pids; do
		is_still_wl_process=`ps -A | grep wl | grep $pid`
		if [ -z $is_wl_process ]; then
			[[ DRY == 1 ]] || kill $pid && echo "Sending SIGTERM to pid: $pid"
		fi
	done
}

trap kill_wl SIGINT SIGTERM

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
