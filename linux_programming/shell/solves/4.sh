echo arg \#0: $0
count=1
for arg in "$@"
do
echo arg \#$count: "$arg"
count=$(($count+1))
done
