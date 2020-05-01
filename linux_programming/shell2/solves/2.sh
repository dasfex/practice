while IFS= read -r url
do
if [[ "$url" =~ ^https?://.*$  ]]
then
        echo -E "$url"
else
        echo -E "http://$url"
fi
done
