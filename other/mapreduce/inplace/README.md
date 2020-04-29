# Inplace MapReduce

This is inplace version of mapreduce for understanding conception.

It consists of three parts:
+ MapReduce(main part).
+ Map.
+ Reduce.

Now Map and Reduce scripts solve WordCount problem.

Run:

```
./mapreduceScript what_we_run path_to_script src_file dst_file
```

For example:

```
./mapreduceScript map mapScript input.txt tmp.txt
./mapreduceScript reduce reduceScript tmp.txt output.txt
```
