#!/bin/bash
echo "Name your hotspot: "
read HOTSPOT_NAME
echo "Type your hotspot password (>= 8 char): "
read HOTSPOT_PWD
echo "Creating your hotspot..."
sudo nmcli device wifi hotspot ssid $HOTSPOT_NAME password $HOTSPOT_PWD ifname wlan0
sudo nmcli connection
echo " "
echo "RaspberryPi hotspot's new network interface: "
ifconfig wlan0
