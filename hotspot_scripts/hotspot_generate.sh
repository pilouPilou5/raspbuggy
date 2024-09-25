#!/bin/bash
echo "Name your hotspot (SSID): "
read HOTSPOT_SSID
echo "Name your connection (NAME): "
read HOTSPOT_NAME
echo "Type your hotspot password (>= 8 char): "
read HOTSPOT_PWD

HOTSPOT_IP="10.214.10.140"

echo "Creating your hotspot..."
sudo nmcli device wifi hotspot ssid $HOTSPOT_SSID password $HOTSPOT_PWD con-name $HOTSPOT_NAME ifname wlan0
echo "Assigning IPv4 address..."
sudo nmcli connection modify id $HOTSPOT_NAME ipv4.addresses 10.214.10.140/24
#sudo nmcli connection modify id $HOTSPOT_NAME ipv4.method manual
echo "Rebooting Hotspot..."
sudo nmcli device connect wlan0
sudo nmcli device connect wlan0
#sudo nmcli connection down $HOTSPOT_NAME
#sudo nmcli connection up $HOTSPOT_NAME
echo "Network info"
nmcli connection show
echo "RaspberryPi hotspot's new network interface: "
ifconfig wlan0
