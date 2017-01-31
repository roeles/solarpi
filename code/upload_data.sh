#!/bin/bash


/sbin/ifup wlan0
sleep 60


MY_PATH="`dirname \"$0\"`"
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"
cd $MY_PATH
cd ..

git pull
make

rsync -z ./data/solarpi.csv -e ssh solarpi@cumulus-humilis.nl:/home/archive/data/solarpi

/sbin/ifdown wlan0 
