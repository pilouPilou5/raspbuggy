/**
 * Usage: ./udp_server <port_no>
 * 
 * UDP server that waits for a client to connects.
 * Reads messsage sent by client, turns it into upper case and sends it back.
 * Disconnects when client sends "bye".
 * 
 * @author  Philippe Lefebvre, ENSICAEN
 * @author  Dimitri Boudier,   ENSICAEN
 */

#include <errno.h>                      // Error management
#include <sys/socket.h>                 // Socket
#include <arpa/inet.h>                  // Internet
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>                     // Removes warning of implicit declaration when closing socket
#include <ctype.h>
#include <fcntl.h>
#include <gpiod.h>
#include <time.h>
#include <pthread.h>

#define MAX_LENGTH 100                  // Maximum length for sent and received messages

// Define the servo min and max pulse lengths in microseconds
#define SERVO_MIN_PULSE 1000  // 1 ms (0 degrees)
#define SERVO_MAX_PULSE 1900  // 2 ms (180 degrees)
#define PWM_PERIOD 19900      // 20 ms (50 Hz)

// Set up the chip and line you want to use for controlling the servo
#define CHIP "/dev/gpiochip4"

// Set the servo input
void sendPWM(struct gpiod_line *line, float input) {
    // Ensure the input is within the 0 to 180 degree range
    if (input < -1) input = -1;
    if (input > 1) input = 1;

    // Calculate the pulse width for the given input
    int pulse_width = SERVO_MIN_PULSE + ((input+1) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 2);

    // Generate the PWM signal
    gpiod_line_set_value(line, 1);  // Set the GPIO pin high
    usleep(pulse_width);    // Wait for the pulse width time (high period)
    gpiod_line_set_value(line, 0);  // Set the GPIO pin low
    usleep(PWM_PERIOD - pulse_width);  // Wait for the remaining period time (low period)
}

void *motorControl(void* p) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int ret;
    float* input = (float*)p;

    // Open the GPIO chip
    chip = gpiod_chip_open(CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        pthread_exit(0);
    }

    // Get the GPIO line
    line = gpiod_chip_get_line(chip, 12);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        pthread_exit(0);
    }

    // Request the line as output
    ret = gpiod_line_request_output(line, "servo-control", 0);
    if (ret < 0) {
        perror("gpiod_line_request_output");
        gpiod_chip_close(chip);
        pthread_exit(0);
    }
    // Loop to move the servo
    while (1) {
        sendPWM(line, *input);
    }

    // Release the line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}

void *servoControl(void* p) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int ret;
    float* input = (float*)p;

    // Open the GPIO chip
    chip = gpiod_chip_open(CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        pthread_exit(0);
    }

    // Get the GPIO line
    line = gpiod_chip_get_line(chip, 13);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        pthread_exit(0);
    }

    // Request the line as output
    ret = gpiod_line_request_output(line, "servo-control", 0);
    if (ret < 0) {
        perror("gpiod_line_request_output");
        gpiod_chip_close(chip);
        pthread_exit(0);
    }
    // Loop to move the servo
    while (1) {
        sendPWM(line, *input);
    }

    // Release the line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}

void main( int argc, char **argv) {
    pthread_t th1, th2;


    // Check the number of arguments : "POC port_no"
    int  server_port_no = 5004;
    

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
    server_address.sin_port         = htons(server_port_no);    // Server port number
    server_address.sin_addr.s_addr  = INADDR_ANY;               // Server can have any IP address


    // Bind socket to listening port
    int socket_error = bind(udp_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
    if (socket_error < 0) {
        perror ("ERROR WHILE BINDING SOCKET:");
        exit(-1);
    }
  

    int  msg_length;
    unsigned int *motor;
    float *servo;
    char message[MAX_LENGTH];
    motor = (unsigned int*)message;
    *motor = 0;
    servo = (float*)(message+sizeof(float));
    *servo = 0;
    char servo_str[100];
    //int pipe = open("/tmp/pipe", O_WRONLY);
    //pthread_create (&th1, NULL, servoControl, (void *)servo) ;
    //pthread_create (&th2, NULL, motorControl, (void *)motor) ;
    //printf ("Server started\n\n");
    do {
        // Read string from client through the dialogue socket
        msg_length = recvfrom(udp_socket, message, MAX_LENGTH, 0, (struct sockaddr *)&server_address,  &address_length);
        //
        // In disconnected mode (UDP)
        // there is no need for checking what has been sent
        // but it can be done if you want to
        //
        
        message[msg_length] = '\0';
        printf("motor : %u  servo : %f\n", *motor, *servo);
        //sprintf(python_cmd, "python3 set_servo.py %f", servo);
        //system(python_cmd);
        //sprintf(servo_str, "%f\n", servo);
        //write(pipe, servo_str, strlen(servo_str));
        
    
    }while ( strncmp(message, "BYE", 3));
    
    printf ("Client has disconnected\r\n");
    
    close(udp_socket);
}