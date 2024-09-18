#include <errno.h>                      // Error management
#include <sys/socket.h>                 // Socket
#include <arpa/inet.h>                  // Internet
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>                     // Removes warning of implicit declaration when closing socket
#include <ctype.h>
#include <fcntl.h>
#include "receive_udp.h"

struct udp_socket_info initUdp(int port_no){
    
    // Open socket in datagram mode (UDP). 17 is UDP number ( cf. file /etc/protocols )
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 17);
    if ( udp_socket < 0 ) {
        perror("\nERROR WHILE OPENING SOCKET:\n");
        exit(-1);
    }
  

    // Set server parameters
    struct sockaddr_in server_address;
    socklen_t address_length = sizeof(struct sockaddr_in);
    server_address.sin_family       = AF_INET;                  // Server address is Internet-type
    server_address.sin_port         = htons(port_no);    // Server port number
    server_address.sin_addr.s_addr  = INADDR_ANY;               // Server can have any IP address


    // Bind socket to listening port
    int socket_error = bind(udp_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
    if (socket_error < 0) {
        perror ("ERROR WHILE BINDING SOCKET:");
        exit(-1);
    }

    struct udp_socket_info output = {udp_socket, server_address, address_length};
    return output;
}

void receiveUdp(struct udp_socket_info socket, char* message){

    int udp_socket = socket.udp_socket;
    struct sockaddr_in server_address = socket.server_address;
    socklen_t address_length = socket.address_length;
    int msg_length;
    
    msg_length = recvfrom(udp_socket, message, MAX_UDP_MESSAGE_LENGTH, 0, (struct sockaddr *)&server_address,  &address_length);
    message[msg_length] = '\0';
}