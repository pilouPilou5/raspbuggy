#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// Define the servo min and max pulse lengths in microseconds
#define SERVO_MIN_PULSE 1000  // 1 ms (0 degrees)
#define SERVO_MAX_PULSE 2000  // 2 ms (180 degrees)
#define PWM_PERIOD 20000      // 20 ms (50 Hz)

// Set up the chip and line you want to use for controlling the servo
#define CHIP "/dev/gpiochip0"
#define LINE 18  // GPIO pin number

// Sleep for a specific number of microseconds
void usleep_wrapper(long us) {
    struct timespec req, rem;
    req.tv_sec = us / 1000000;
    req.tv_nsec = (us % 1000000) * 1000;
    nanosleep(&req, &rem);
}

// Set the servo angle
void set_servo_angle(struct gpiod_line *line, int angle) {
    // Ensure the angle is within the 0 to 180 degree range
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // Calculate the pulse width for the given angle
    int pulse_width = SERVO_MIN_PULSE + (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180);

    // Generate the PWM signal
    gpiod_line_set_value(line, 1);  // Set the GPIO pin high
    usleep_wrapper(pulse_width);    // Wait for the pulse width time (high period)
    gpiod_line_set_value(line, 0);  // Set the GPIO pin low
    usleep_wrapper(PWM_PERIOD - pulse_width);  // Wait for the remaining period time (low period)
}

int main() {
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

    // Loop to move the servo
    while (1) {
        // Sweep from 0 to 180 degrees
        for (int angle = 0; angle <= 180; angle += 10) {
            set_servo_angle(line, angle);
            usleep_wrapper(500000);  // Wait 0.5 seconds between steps
        }

        // Sweep back from 180 to 0 degrees
        for (int angle = 180; angle >= 0; angle -= 10) {
            set_servo_angle(line, angle);
            usleep_wrapper(500000);  // Wait 0.5 seconds between steps
        }
    }

    // Release the line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return 0;
}