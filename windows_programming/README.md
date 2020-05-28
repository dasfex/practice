# How to use pipeline

This call
```
pipeline.exe ipconfig /all "|" find /i "\"TCP/IP. .\"" "|" find /c "\"Next\""
```

are equal to
```
ipconfig /all | find /i | "TCP/IP. ." " find /c "Net"
```
