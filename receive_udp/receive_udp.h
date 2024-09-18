#ifndef RECEIVE_UDP_H
#define RECEIVE_UDP_H

#include <sys/socket.h>                 // Socket
#include <arpa/inet.h>                  // Internet

#define MAX_UDP_MESSAGE_LENGTH 100

struct udp_socket_info{
    int udp_socket;
    struct sockaddr_in server_address;
    socklen_t address_length;
};

struct udp_socket_info initUdp(int port_no);

void receiveUdp(int port_no, char* buffer);

#endif