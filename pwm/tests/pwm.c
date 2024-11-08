#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// Define the servo min and max pulse lengths in microseconds
#define SERVO_MIN_PULSE 1000  // 1 ms (0 degrees)
#define SERVO_MAX_PULSE 2000  // 2 ms (180 degrees)
#define PWM_PERIOD 20000      // 20 ms (50 Hz)

// Set up the chip and line you want to use for controlling the servo
#define CHIP "/dev/gpiochip4"
#define LINE 13  // GPIO pin number

// Set the servo input
void set_servo(struct gpiod_line *line, float input) {
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

int main(int argc, char** argv) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int ret;

    // Open the GPIO chip
    chip = gpiod_chip_open(CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    // Get the GPIO line
    line = gpiod_chip_get_line(chip, LINE);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return 1;
    }

    // Request the line as output
    ret = gpiod_line_request_output(line, "servo-control", 0);
    if (ret < 0) {
        perror("gpiod_line_request_output");
        gpiod_chip_close(chip);
        return 1;
    }
    float input;
    sscanf(argv[1], "%f", &input);
    //printf("%f", input);
    // Loop to move the servo
    while (1) {
        set_servo(line, input);
    }

    // Release the line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return 0;
}