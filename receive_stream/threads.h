#ifndef THREADS_H
#define THREADS_H

#include  <gst/gst.h>

// Define specific IP address and port
#define IP_ADDRESS "10.42.0.73"  // Replace with your IP address
#define PORT "5000"                // Replace with your port number



void *gst_thread_receive(void *arg);





#endif