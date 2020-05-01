first() {
    find ./* -type f -name "*.$1" -print0 | xargs -0 sed -i "s/ $2( *\"\(.*\)\" *)/ u\"\1\"/g"
}

second() {
    find ./* -type f -name "*.$1" -print0 | xargs -0 sed -i "s/^$2( *\"\(.*\)\" *)/u\"\1\"/g"
}

first h ASCIIToWide
first cpp ASCIIToWide
first h UTF8ToWide
first cpp UTF8ToWide
first h Utf16StringFor
first cpp Utf16StringFor

second h ASCIIToWide
second cpp ASCIIToWide
second h UTF8ToWide
second cpp UTF8ToWide
second h Utf16StringFor
second cpp Utf16StringFor
