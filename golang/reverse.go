package reverse

import (
	"strings"
	"unicode/utf8"
)

func Reverse(input string) string {
	var buf strings.Builder
	buf.Grow(len(input))
	for len(input) > 0 {
		r, sz := utf8.DecodeLastRuneInString(input)
		buf.WriteRune(r)
		input = input[:len(input)-sz]
	}
	return buf.String()
}
