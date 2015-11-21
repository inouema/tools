#!/bin/sh

set -ux
LANG=C

# $1: community name
# $2: dist ip address
# $3: dist port num
# $4: message

#sudo snmptrap -v 1 -c public 192.168.11.32:162 .1.3.6.1.4.1.36893 '' 6 4 '' .1.3.6.1.4.1.36893 s "0x112233445566"
sudo snmptrap -v 1 -m ./TAKACOM-MIB-TS500  -c $1 $2:$3 .1.3.6.1.4.1.36893 '' 0 0 ''
                             # to               takacom enterprise.oid         host ? ? uptime oid  type msg
