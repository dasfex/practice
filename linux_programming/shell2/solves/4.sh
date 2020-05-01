path="$*"
find $path -name "*.[hc]" -exec cat {} \; | wc -l
