n=$1
if [ $n -eq 0 ]
then
exit 1
fi
if [ $n -eq 1 ]
then
exit 1
fi
div=2
pr=$[$div*$div]
while [ $pr -le $n ]
do
        mod=$[ $n % $div ]
        if [ $mod -eq 0 ]
        then
                exit 1
        fi
div=$[$div+1]
pr=$[$div*$div]
done
exit 0
