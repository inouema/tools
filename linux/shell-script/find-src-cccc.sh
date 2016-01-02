#!/bin/sh


# 拡張子 cppとcc のソースコードを「cccc」でメトリクス解析を行う
find . -type f \( -name '*.cpp' -o -name '*.cc' \) | cccc - --outdir="./cccc_metrics_report"

