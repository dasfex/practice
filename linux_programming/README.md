# Additional info

## How to use cat?

```
gcc -o cat cat.c
./cat <<<"DasfEX"
DasfEX
./cat f.txt
1
2
3
./cat f{1,2}.txt
1 2
3
Can't open file
```

## How to use pipeline?

```
gcc -o pipe pipeline.c
```

Now this run:

```
./pipe ls -l -a \| wc -l
```

are equal to:

```
ls -l -a | wc -l
```
