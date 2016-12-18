#!/bin/bash

MY_PATH="`dirname \"$0\"`"
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"
cd $MY_PATH
cd ..


./code/enable_usb.sh
sleep 10
./bin/scdplus /dev/ttyACM0 9600 >> ./data/solarpi.csv
./code/disable_usb.sh

