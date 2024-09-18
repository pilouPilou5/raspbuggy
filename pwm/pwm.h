#ifndef PWM_H
#define PWM_H

#include <gpiod.h>

// Define the motor and servo min and max pulse lengths in microseconds
#define MOTOR_MIN_PULSE 1000
#define MOTOR_MAX_PULSE 1900
#define SERVO_MIN_PULSE 1000
#define SERVO_MAX_PULSE 1900
#define PWM_PERIOD 19900

// Set up the chip and line you want to use for controlling the servo
#define GPIO_CHIP "/dev/gpiochip4"

#define MOTOR_LINE 12
#define SERVO_LINE 13

void sendPWM(struct gpiod_line *line, float input);

struct gpiod_chip* initChip(char* chip_number);

struct gpiod_line* initLine(struct gpiod_chip* chip, int line_number);

#endif