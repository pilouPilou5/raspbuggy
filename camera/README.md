


     
These are GStreamer to send video stream through udp protocol

### Set up for camera use

Ensure the operating system is up to date by using these two commands: 

`sudo apt-get update
sudo apt-get upgrade`

Then run the config tool:

`sudo raspi-config`

Select the 'Enable Camera' option and set it to Enabled.

Exit the config tool and after it reboots enter the following command to test the camera:

`rpicam-hello`

A preview of the camera should be shown for a few seconds.
### Install Gstreamer in raspberry pi
 install a missing dependency
`sudo apt-get install libx264-dev libjpeg-dev`

install the remaining plugins

` sudo apt-get install libgstreamer1.0-dev \
     libgstreamer-plugins-base1.0-dev \
     libgstreamer-plugins-bad1.0-dev \
     gstreamer1.0-plugins-ugly \
     gstreamer1.0-tools \
     gstreamer1.0-gl \
     gstreamer1.0-gtk3`

 if you have Qt5 install this plugin

` sudo apt-get install gstreamer1.0-qt5`

add libraries:

`sudo apt update
sudo apt install libcamera-dev` and `sudo apt install gstreamer1.0-libcamera`

### install GStreamer in Ubuntu
`sudo apt install v4l-utils`



`sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio`

add libraries:

`sudo apt update
sudo apt install libcamera-dev` and `sudo apt install gstreamer1.0-libcamera`





  



