#!/bin/sh

set -eux
LANG=C


# stop snmpd
sudo /etc/init.d/snmpd stop

#
# change community name
#

# delate "com2sec" line
sudo sed -i -e '/com2sec/d' /etc/snmp/snmpd.conf

# add "com2sec" line at new community name
sudo sed -i -e "9i com2sec ts500_sec default $1" /etc/snmp/snmpd.conf

# start snmpd
sudo /etc/init.d/snmpd start

#$?
