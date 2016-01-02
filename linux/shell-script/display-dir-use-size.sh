#!/bin/sh


# 各ディレクトリの利用容量を一覧表示する
sudo find /home -mindepth 1 -maxdepth 1 -type d -exec du -hs {} \;

