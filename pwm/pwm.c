#include <gpiod.h>
#include <unistd.h>
#include <stdio.h>
#include "pwm.h"

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

struct gpiod_chip* initChip(char* chip_number){

    struct gpiod_chip* chip;
    chip = gpiod_chip_open(chip_number);
    if (!chip) {
        perror("gpiod_chip_open");
    }
    return chip;
}

struct gpiod_line* initLine(struct gpiod_chip* chip, int line_number){

    struct gpiod_line* line;
    line = gpiod_chip_get_line(chip, line_number);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
    }

    // Request the line as output
    int ret = gpiod_line_request_output(line, "servo-control", 0);
    if (ret < 0) {
        perror("gpiod_line_request_output");
        gpiod_chip_close(chip);
    }
    return line;
}