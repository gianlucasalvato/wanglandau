#!/bin/bash

in_filename="dos_formatted.txt"
out_filename="doslog_formatted.txt"
work_directory=`pwd`"/work"

in_filepath="$work_directory/$1/$in_filename"

if [ ! -f "$in_filepath" ]; then
	echo "No valid file found."
	exit 1
fi

cat "$in_filepath" | awk -F"\t" -v 'OFS=\t' '{ $3=""; print $0}' | sed 's/\t\{2,\}/\t/' > "$work_directory/$1/$out_filename"

exit 0