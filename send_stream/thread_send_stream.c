#include "threads.h"
#include <stdlib.h>
#include <stdio.h>
#include <gst/gst.h>

void gst_thread_send(void){
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    char command[512];


    // Construct the GStreamer pipeline command with specific IP and port
    snprintf(command, sizeof(command),
    "libcamerasrc ! video/x-raw,format=I420,width=640,height=480,framerate=30/1 ! jpegenc ! jpegparse ! rtpjpegpay ! queue ! udpsink host=%s port=%s",
    IP_ADDRESS, PORT);
    //Create pipeline
    pipeline = gst_parse_launch( command, NULL);
    if (!pipeline) {
        g_printerr("Failed to create pipeline.\n");
    }

    // Set the pipeline to the PLAYING state
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Failed to set pipeline to playing state.\n");
        gst_object_unref(pipeline);
    }
    // Get the bus and wait for EOS or ERROR message
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    // Handle the received message
    if (msg != NULL) {
        if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
            GError *err;
            gchar *debug_info;
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error: %s\n", err->message);
            g_free(debug_info);
            g_error_free(err);
        } else if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_EOS) {
            g_print("End of stream reached.\n");
        }
        gst_message_unref(msg);
    }

    // Clean up
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}