#!/bin/sh

set -ux

LANG=C

echo $1
echo $2

ifconfig eth0 $1 netmask $2
