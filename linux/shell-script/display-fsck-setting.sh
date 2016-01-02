#!/bin/sh


# fsckの実行周期を表示します。
# 項目：
# Maximum mount count
# Check interval
sudo tune2fs -l /dev/sda1 | less

