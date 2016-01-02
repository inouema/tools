#
# zcat grep
# 説明: zcat は 圧縮されたファイルを展開しながら読み出すコマンド
# 含まない行だけを出力
zcat file.2.gz | grep  "hogehoge" | grep -v "piyo" | less
