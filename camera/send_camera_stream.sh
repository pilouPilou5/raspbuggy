#!/bin/bash

# Check if both IP address and port are provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <IP_ADDRESS> <PORT>"
    exit 1
fi

# Assign input arguments to variables
IP_ADDRESS=$1
PORT=$2

# Execute the GStreamer pipeline
echo "START STREAMING"

gst-launch-1.0 libcamerasrc ! \
    video/x-raw,format=I420,width=640,height=480,framerate=30/1 ! \
    jpegenc ! jpegparse ! rtpjpegpay ! \
    queue ! \
    udpsink host=$IP_ADDRESS port=$PORT
