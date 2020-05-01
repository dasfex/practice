awk '{
  if (substr($1,1,1) != "#" && $4 == "GET") {
        ++mp[$1]
  }
}
END {
  n = asorti(mp, ind)
  for (i = 1; i <= n; ++i) {
    print ind[i]"\t"mp[ind[i]]
  }
}'
