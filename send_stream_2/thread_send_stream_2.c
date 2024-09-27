#include "threads.h"
#include <stdlib.h>
#include <stdio.h>
#include <gst/gst.h>

void* gst_thread_send_2(void* arg) {
    // GStreamer pipeline command as a shell script
    const char* gst_command = 
        "gst-launch-1.0 compositor name=mix sink_0::xpos=0 sink_0::ypos=0 sink_1::xpos=640 sink_1::ypos=0 ! "
        "video/x-raw,width=1280,height=480 ! videoconvert ! jpegenc ! jpegparse ! rtpjpegpay ! "
        "udpsink host=127.0.0.1 port=5000 "
        "libcamerasrc camera-name='" CAMERA0 "' ! "
        "video/x-raw,width=640,height=480,framerate=30/1,format=I420 ! queue ! videoconvert ! mix.sink_0 "
        "libcamerasrc camera-name='" CAMERA1 "' ! "
        "video/x-raw,width=640,height=480,framerate=30/1,format=I420 ! queue ! videoconvert ! mix.sink_1";


    
    // Execute the GStreamer pipeline command
    int ret = system(gst_command);
    
    if (ret == -1) {
        perror("Error executing GStreamer pipeline");
    } else {
        printf("GStreamer pipeline executed successfully\n");
    }

    return NULL;
}