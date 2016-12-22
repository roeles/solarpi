#!/bin/bash


/sbin/ifup wlan0


MY_PATH="`dirname \"$0\"`"
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"
cd $MY_PATH
cd ..

git pull
make

rsync ./data/solarpi.csv rsync://192.168.178.36/data/solarpi

sleep 60
/sbin/ifdown wlan0 
