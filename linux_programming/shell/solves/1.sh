if [ -n "$*" ]
then
echo 'Hello, world!' > "$*"
else
echo 'Hello, world!' > readme.txt
fi
