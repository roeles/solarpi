#!/bin/bash

MY_PATH="`dirname \"$0\"`"
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"
#echo $MY_PATH
cd $MY_PATH
cd ..

./code/enable_usb.sh
sleep 10
./bin/scdplus /dev/ttyACM0 9600 | nc 192.168.178.36 1234 
./code/disable_usb.sh
