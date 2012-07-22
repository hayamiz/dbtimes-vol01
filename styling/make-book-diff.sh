#!/bin/bash

files=""
para=0

for page in $(seq 1 44); do
	pdftk ../book-ready.pdf cat $page output new$page.pdf
	pdftk ../book-ready-dist.pdf cat $page output dist$page.pdf
	pdftk new$page.pdf stamp dist$page.pdf output diff$page.pdf
	files="$files diff${page}.pdf"
done

pdftk $files cat output diff.pdf

for page in $(seq 1 44); do
	rm diff${page}.pdf
done

rm new*.pdf dist*.pdf