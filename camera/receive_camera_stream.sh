#!/bin/bash

# Check if the port is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <PORT>"
    exit 1
fi

# Assign input argument to variable
PORT=$1

# Execute the GStreamer pipeline
gst-launch-1.0 udpsrc port=$PORT caps="application/x-rtp, media=(string)video, encoding-name=(string)JPEG" ! \
    rtpjpegdepay ! jpegdec ! videoconvert ! queue ! autovideosink

