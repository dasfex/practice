# Web-robot(mr.pdf)

It is a realisation of bfs over the internet.

We goes from all links from input file to the specified depth. We don't visit the same site twice. All links are absolute and comes from \<a href\> tags.

Run as
```
./run.sh input_file output_file file_for_log tmp_dir_name depth map_count
```

For example
```
./run.sh i1.txt o1.txt log1 tmp 2 10
```
