#ifndef THREADS_H
#define THREADS_H

#include  <gst/gst.h>

// Define specific IP address and port
#define IP_ADDRESS "10.42.0.73"  // Replace with your IP address
#define PORT "5000"                // Replace with your port number
#define CAMERA0 "'/base/axi/pcie@120000/rp1/i2c@88000/imx708@1a'"
#define CAMERA1 "'/base/axi/pcie@120000/rp1/i2c@80000/imx708@1a'"     //replace with with your cameras device name


void *gst_thread_test(void *arg);
void *gst_thread_send(void *arg);
void *gst_thread_send_2(void *arg);






#endif
