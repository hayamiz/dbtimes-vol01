#!/bin/bash

files=""
para=0

for page in $(seq 1 41); do
	next_page=$((page+1))
	pdftk page$page.pdf stamp page$next_page.pdf output match$page.pdf &
	files="$files match${page}.pdf"
	if [ $para -lt 8 ]; then
		para=$((para+1))
	else
		wait
		para=0
	fi
done

pdftk $files cat output baseline-match.pdf
# rm match*.pdf page*.pdf