#!/bin/bash

#sudo cp wifiStartup.service /etc/systemd/system/
#sudo systemctl daemon-reload
#sudo systemctl enable wifiStartup.service 
#sudo systemctl start wifiStartup.service 
#sudo systemctl status wifiStartup.service

#Enter your Wifi Credentials in this file: sudo vim /etc/wpa_supplicant/wpa_supplicant.conf

sudo ifconfig wlan0 down

sudo ifconfig wlan0 up

sudo wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf

sudo dhclient wlan0
