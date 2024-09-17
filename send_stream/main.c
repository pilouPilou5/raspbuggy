#include <pthread.h>
#include <errno.h>
#include "threads.h"
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char* argv[]){
    
    pthread_t thread_send;
    int ret;

    //Initialize GStreamer
    gst_init(&argc, &argv);

    if ( pthread_create(&thread_send,NULL,gst_thread_send,"1")){
        perror("pb thread_create");
        exit(1);
    }

    pthread_join(thread_send, (void *)&ret) ;


}