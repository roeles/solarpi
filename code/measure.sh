#!/bin/bash

date

MY_PATH="`dirname \"$0\"`"
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"
cd $MY_PATH
cd ..

./code/disable_hdmi.sh
./code/enable_usb.sh
sleep 3
./bin/scdplus /dev/ttyACM0 9600 >> ./data/solarpi.csv
./code/disable_usb.sh

