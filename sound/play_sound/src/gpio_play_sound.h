#ifndef GPIO_PWM_H
#define GPIO_PWM_H

#define SOUND_LINE 18
// Function to play a sound from a WAV file via PWM
// The parameters are: the path to the WAV file, the GPIO chip, and the GPIO line
void play_sound(const char *filename, const char *gpio_chip, int gpio_line);

#endif // GPIO_PWM_H
