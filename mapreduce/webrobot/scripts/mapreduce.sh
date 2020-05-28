#! /bin/bash

src="$1"
dst="$2"
log="$3"
tmp_dir="$4"
depth=$5
maps=$6

mkdir "$tmp_dir"

tp="$tmp_dir/tmp.txt"
tp1="$tmp_dir/tmp1.txt"

cp "$src" "$tp1"

for i in $(seq 1 $depth)
do
./mapreduce map map.py "$tp1" "$tp" $5 2>"$log"
./mapreduce reduce reduce.py "$tp" "$tp1" compare.py
done

./mapreduce map mapNormalize.py "$tp1" "$dst" $5

rm -rf "$tmp_dir"
