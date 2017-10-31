#!/bin/bash

date

/sbin/ifup wlan0
sleep 60

SERVER=8.8.8.8
ping -c10 ${SERVER}

if [ $? != 0 ]
then
	/sbin/shutdown -r now
	exit 1
fi


MY_PATH="`dirname \"$0\"`"
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"
cd $MY_PATH
cd ..

git pull
#make

rsync -z ./data/solarpi.csv -e ssh solarpi@baardman.net:/home/archive/data/solarpi
rsync -z ./data/solarpi.log -e ssh solarpi@baardman.net:/home/archive/data/solarpi

/sbin/ifdown --force wlan0 
