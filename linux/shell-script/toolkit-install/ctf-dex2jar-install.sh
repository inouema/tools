#!/bin/sh
set -eux
export LANG=C

wget http://sourceforge.net/projects/dex2jar/files/dex2jar-2.0.zip
unzip dex2jar-2.0.zip
cd dex2jar-2.0
chmod u+x ./*.sh
#export PATH=$PATH:/path/to/dex2jar
