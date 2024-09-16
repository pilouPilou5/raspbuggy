#!/bin/bash

# get the dependencies
echo "GET DEPENDENCIES"
sudo apt-get install wget git make gcc

# get the git
echo "GET GIT"
$MYPATH=$PWD+'rpi-rtl8812au'
git clone https://github.com/emmercm/rpi-rtl8812au

# adapt script
#chmod +x  $PATH/rpi-rtl8812au
echo "ADAPT SCRIPT"
chmod +x $PWD/rpi-rtl8812au/rpi-rtl8812au
./rpi-rtl8812au/rpi-rtl8812au
