#include <gpiod.h>
#include <stdio.h>
#include <pthread.h>
#include "pwm.h"

struct pwm_thread_input{
    struct gpiod_line* line;
    float* command;
};

void *pwm_thread(void* p){
    struct pwm_thread_input input = *(struct pwm_thread_input*)p;
    struct gpiod_line* line = input.line;
    float* command = input.command;
    while(1){
        sendPWM(line, *command);
    }
}

int main(){
    pthread_t motor_thread, servo_thread;
    float motor_command[1], servo_command[1];

    struct gpiod_chip* chip = initChip(GPIO_CHIP);
    struct gpiod_line* motor_line = initLine(chip, MOTOR_LINE);
    struct gpiod_line* servo_line = initLine(chip, SERVO_LINE);
    *motor_command = 0;
    *servo_command = 0.5;
    struct pwm_thread_input motor_thread_input = {motor_line, motor_command};
    struct pwm_thread_input servo_thread_input = {servo_line, servo_command};

    pthread_create (&motor_thread, NULL, pwm_thread, (void *)&motor_thread_input);
    pthread_create (&servo_thread, NULL, pwm_thread, (void *)&servo_thread_input);
    while(1);
    return 0;
}