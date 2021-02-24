/*
myfmt.Sprintf("{} h{5}lo {3} {} {} {}", []interface{}{0, 1, 2, 3, 4, "el"}) == "0 hello 3 2 3 4"
*/

package myfmt

import (
	"bytes"
	"reflect"
	"strconv"
)

func Sprintf(format string, args ...interface{}) string {
	var buf bytes.Buffer
	runes := []rune(format)
	ind := 0
	for l, r := 0, 0;; {
		for l < len(runes) && runes[l] != '{' {
			buf.WriteRune(runes[l])
			l++
		}
		if l == len(runes) {
			break
		}
		r = l + 1
		num := 0
		for runes[r] != '}' {
			num = num * 10 + int(runes[r] - '0')
			r++
		}
		writeInd := ind
		if r - l != 1 {
			writeInd = num
		}
		if reflect.TypeOf(args[writeInd]).Kind() == reflect.Int {
			buf.WriteString(strconv.Itoa(args[writeInd].(int)))
		} else {
			buf.WriteString(args[writeInd].(string))
		}
		ind++
		l = r + 1
	}

	return buf.String()
}
