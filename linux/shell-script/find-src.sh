#!/bin/sh


# 拡張子 cppとcc のファイルリストを表示して書き出す
find . -type f \( -name '*.cpp' -o -name '*.cc' \) -print > source.list

