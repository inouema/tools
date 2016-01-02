#
# grep scrpit
# 説明: grep結果をパイプし、更にgrep オプション(-v:指定した文字列を
# 含まない行だけを出力
grep  "hogehoge" /path/to/file | grep -v "piyo" | less
