#ifndef GPIO_PWM_H
#define GPIO_PWM_H

// Fonction pour jouer un son depuis un fichier WAV via PWM
// Les paramètres sont : le chemin du fichier WAV, le chip GPIO, et la ligne GPIO
void play_sound(const char *filename, const char *gpio_chip, int gpio_line);

#endif // GPIO_PWM_H
