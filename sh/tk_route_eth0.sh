#!/bin/sh

set -ux

LANG=C

echo $1

route add default gw $1
