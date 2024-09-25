#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sndfile.h>
#include "gpio_play_sound.h"

#define SAMPLE_RATE 44100
#define AMPLITUDE_MAX 100

static inline long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

void play_sound(const char *filename, const char *gpio_chip, int gpio_line) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    struct gpiod_line_request_config config;
    int ret;
    long time_step_ns = (1.0 / SAMPLE_RATE) * 1e9;
    long on_time_ns, off_time_ns;
    long start_time, current_time;

    // Open the WAV audio file
    SF_INFO sfinfo;
    SNDFILE *sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile) {
        fprintf(stderr, "Error: unable to open audio file %s\n", filename);
        return;
    }

    // Check that the audio file is in mono format
    if (sfinfo.channels != 1) {
        fprintf(stderr, "Error: audio file must be mono.\n");
        sf_close(sndfile);
        return;
    }

    // Allocate a buffer for audio samples
    float buffer[sfinfo.frames];

    // Read the audio samples
    sf_read_float(sndfile, buffer, sfinfo.frames);
    sf_close(sndfile);

    // Open the GPIO chip
    chip = gpiod_chip_open(gpio_chip);
    if (!chip) {
        perror("gpiod_chip_open");
        return;
    }

    // Get the GPIO line
    line = gpiod_chip_get_line(chip, gpio_line);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return;
    }

    // Initialize the GPIO line configuration
    memset(&config, 0, sizeof(config));
    config.consumer = "audio_pwm";
    config.request_type = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT;

    // Request access to the GPIO line
    ret = gpiod_line_request(line, &config, 0);
    if (ret < 0) {
        perror("gpiod_line_request");
        gpiod_chip_close(chip);
        return;
    }

    // Loop to send audio data as PWM signal
    for (long i = 0; i < sfinfo.frames; i++) {
        float sample = buffer[i];
        float duty_cycle = ((sample + 1.0) / 2.0) * AMPLITUDE_MAX;
        on_time_ns = (duty_cycle / AMPLITUDE_MAX) * time_step_ns;
        off_time_ns = time_step_ns - on_time_ns;

        // Turn the pin ON
        gpiod_line_set_value(line, 1);
        start_time = get_time_ns();
        do {
            current_time = get_time_ns();
        } while ((current_time - start_time) < on_time_ns);

        // Turn the pin OFF
        gpiod_line_set_value(line, 0);
        start_time = get_time_ns();
        do {
            current_time = get_time_ns();
        } while ((current_time - start_time) < off_time_ns);
    }

    // Release the GPIO resources
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}
