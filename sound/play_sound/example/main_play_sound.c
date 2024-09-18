#include <stdio.h>
#include "gpio_play_sound.h"

int main() {
    // Remplacez ces valeurs par celles correspondant à votre configuration
    const char *filename = "soud.wav";
    const char *gpio_chip = "/dev/gpiochip4";
    int gpio_line = 18;

    // Jouer le son 
    play_sound(filename, gpio_chip, gpio_line);

    return 0;
}
