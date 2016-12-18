#!/bin/bash

ifup wlan0
sleep 10

MY_PATH="`dirname \"$0\"`"
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"
cd $MY_PATH
cd ..

rsync ./data/solarpi.csv rsync://192.168.178.36/data/solarpi

ifdown wlan0
