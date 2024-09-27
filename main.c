#include <gpiod.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "pwm.h"
#include "receive_udp.h"
#include "process_controller_inputs.h"
#include "wifi_api.h"
#include "threads.h"
#include "gpio_play_sound.h"

struct pwm_thread_input{
    struct gpiod_line* line;
    float* command;
};

struct sound_thread_input{
    char* filename;
    bool* sound_playing;
};

void *pwmThread(void* p){
    struct pwm_thread_input input = *(struct pwm_thread_input*)p;
    struct gpiod_line* line = input.line;
    float* command = input.command;
    while(1){
        sendPWM(line, *command);
    }
}

void *streamThread(void* p){
    gst_init(NULL, NULL);
    gst_thread_send();
}

void *soundThread(void* p){
    struct sound_thread_input input = *(struct sound_thread_input*)p;
    char* filename = input.filename;
    bool* sound_playing = input.sound_playing;
    *sound_playing = 1;
    play_sound(filename, GPIO_CHIP, SOUND_LINE);
    *sound_playing = 0;
}

void *ledThread(void* p){
    system("bin/rainbow");
}

int main(){
    //hotspot
    generate_hotspot("raspbuggy", "raspbuggy", "12345678");

    //pwm
    pthread_t motor_thread, servo_thread, stream_thread, sound_thread;

    float motor_command[1], servo_command[1];

    struct gpiod_chip* chip = initChip(GPIO_CHIP);
    struct gpiod_line* motor_line = initLine(chip, MOTOR_LINE);
    struct gpiod_line* servo_line = initLine(chip, SERVO_LINE);

    *motor_command = 0;
    *servo_command = 0;
    struct pwm_thread_input motor_thread_input = {motor_line, motor_command};
    struct pwm_thread_input servo_thread_input = {servo_line, servo_command};

    pthread_create(&motor_thread, NULL, pwmThread, (void *)&motor_thread_input);
    pthread_create(&servo_thread, NULL, pwmThread, (void *)&servo_thread_input);

    //camera
    pthread_create(&stream_thread, NULL, streamThread, NULL);

    //sound
    char filename[100];
    bool sound_playing;
    struct sound_thread_input sound_input = {filename, &sound_playing};

    //led
    pthread_t led_thread;
    pthread_create(&led_thread, NULL, ledThread, NULL);

    //udp reception
    struct udp_socket_info socket = initUdp(5004);
    char message[MAX_UDP_MESSAGE_LENGTH];
    struct controller_inputs inputs;
    
    while(1){
        receiveUdp(socket, message);
        inputs = processControllerInputs(message);
        *motor_command = (inputs.rt-inputs.lt)/2;
        *servo_command = -0.28*inputs.left_joystick_x_axis;
        if(inputs.a){
            if(!sound_playing){
                sprintf(filename, "sound/files/liberty_city_horn.wav");
                pthread_create(&sound_thread, NULL, soundThread, (void *)&sound_input);
            }
        }
    }
    return 0;
}