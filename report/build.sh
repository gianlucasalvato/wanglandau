#!/bin/bash

if [ ! -e "$1.tex" ]; then echo "Could not find $1.tex. Quit."; exit 1; fi
BASENAME="$1"

exec 3>&1
exec 4>&2
if [ "$2" != "-v" ]; then exec 1>/dev/null; exec 2>/dev/null; fi

if [ -e "$BASENAME.pdf" ]; then rm "$BASENAME.pdf"; fi
if [ ! -d './tmp' ]; then mkdir './tmp'; fi

echo "Building first $BASENAME to get auxiliary files..." 1>&3 2>&4

pdflatex -output-directory ./tmp "$BASENAME.tex" | grep --color -E "Warning|Missing|$"

if test ${PIPESTATUS[0]} -eq 0; then
	echo 'Successful! Building bibliography...' 1>&3 2>&4
	bibtex "./tmp/$BASENAME"
	echo "Done. Building second $BASENAME to get bibliography right..." 1>&3 2>&4
	pdflatex -output-directory './tmp' "$BASENAME.tex"
	echo "Done. Building third $BASENAME to get cross-references right..." 1>&3 2>&4
	pdflatex -output-directory './tmp' "$BASENAME.tex" | grep -A 1 --color -E "Warning|Missing" 1>&3 2>&4
	mv ./{tmp/,}$BASENAME.bbl
	echo "Done." 1>&3 2>&4
else
	echo "Cannot compile LaTeX source. Auxiliary files will not be deleted,"\
		 "check $BASENAME.log for details. Quit." 1>&3 2>&4
	exit 1
fi

if [ $? == 0 ]; then mv ./{tmp/,}$BASENAME.pdf; rm -R './tmp'; fi

exit 0