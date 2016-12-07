#!/bin/bash

MY_PATH="`dirname \"$0\"`"
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"
#echo $MY_PATH
cd $MY_PATH
cd ..

git pull
make


./code/enable_usb.sh
sleep 10
./bin/scdplus /dev/ttyACM0 9600 >> ./data/solarpi.csv
./code/disable_usb.sh
rsync ./data/solarpi.csv rsync://192.168.178.36/data/solarpi

