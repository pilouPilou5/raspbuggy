#!/bin/bash
sudo nmcli connection
echo "Type the name of the hotspot you wish to delete (under NAME) "
read HOTSPOT_NAME
sudo nmcli connection delete $HOTSPOT_NAME
