makeForm()
{
if [ $1 -eq 1 ]
then
if [ $2 -ne 1 ]
then
        dir="directories"
fi
else
if [ $2 -ne 1 ]
then
        file="files"
fi
fi
}

path="$*"
file="file"
dir="directory"
files="$(find "${path}" -type f | wc -l)"
makeForm 2 $files
dirs="$(find "${path}" -type d | wc -l)"
makeForm 1 $dirs
echo $files $file
echo $dirs $dir
