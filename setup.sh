#!/bin/bash

#update
yes | sudo apt update
yes | sudo apt upgrade

#install gpiod library
yes | sudo apt install libgpiod-dev

#install gstream and camera drivers
yes | sudo apt install libx264-dev libjpeg-dev
yes | sudo apt install libgstreamer1.0-dev
yes | sudo apt install libgstreamer-plugins-base1.0-dev
yes | sudo apt install libgstreamer-plugins-bad1.0-dev
yes | sudo apt install gstreamer1.0-plugins-ugly
yes | sudo apt install gstreamer1.0-tools
yes | sudo apt install gstreamer1.0-gl
yes | sudo apt install gstreamer1.0-gtk3
yes | sudo apt install libcamera-dev
yes | sudo apt install gstreamer1.0-libcamera

#activate spi peripherals
echo "dtparam=spi=on" >> /boot/firmware/config.txt
echo "dtoverlay=spi1-1cs" >> /boot/firmware/config.txt
echo "dtoverlay=spi0-1cs" >> /boot/firmware/config.txt