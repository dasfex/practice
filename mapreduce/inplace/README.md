# Inplace MapReduce

This is inplace version of mapreduce for understanding conception.

It consists of three parts:
+ MapReduce(main part).
+ Map.
+ Reduce.

Now Map and Reduce scripts solve WordCount problem.
```
key1 \t qwe qwe qwe re
key2 \t asd asd
key1 \t re
a \t asd asd
```

After map(possible output):
```
qwe 1
qwe 1
qwe 1
re 1
asd 1
asd 1
re 1
asd 1
asd 1
```

After reduce(possible output):
```
qwe 3
asd 4
re 2
```

Run:

```
./mapreduceScript what_we_run path_to_script src_file dst_file
```

For example:

```
./mapreduceScript map mapScript input.txt tmp.txt
./mapreduceScript reduce reduceScript tmp.txt output.txt
```
