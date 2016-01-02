#!/bin/sh
set -eux
export LANG=C
sudo apt-get install build-essential
sudo apt-get install gawk
sudo apt-get install gettext
sudo apt-get install texinfo
sudo apt-get install m4
sudo apt-get install flex
#sudo apt-get install uboot-mkimage # for Ubuntu 12.04
sudo apt-get install u-boot-tools # for Ubuntu 14.04
sudo apt-get install texlive
sudo apt-get install bison
sudo apt-get install libncurses5-dev
sudo apt-get install automake
sudo apt-get install libtool
sudo apt-get install dialog
sudo apt-get install lzop
sudo apt-get install git
