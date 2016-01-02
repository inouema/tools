#!/bin/sh
#
# busyboxのudpsvd経由でtftpdを起動させる
udpsvd -vE 0.0.0.0 69 tftpd /file/to/serve
