#include <stdio.h>
#include <stdint.h>
#include <ws2811.h>
#include <unistd.h> // Pour sleep()

#define LED_COUNT 8
#define GPIO_PIN 21  // Choisir un GPIO différent pour PCM
#define TARGET_FREQ WS2811_TARGET_FREQ
#define DMA 10

ws2811_t ledstring = {
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel = {
        [0] = {
            .gpionum = GPIO_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = 255,
            .strip_type = WS2811_STRIP_GRB,  // Format GRB pour NeoPixel
        }
    }
};

int main() {
    // Initialiser la bibliothèque
    if (ws2811_init(&ledstring) != WS2811_SUCCESS) {
        fprintf(stderr, "Erreur d'initialisation du ws2811\n");
        return -1;
    }

    // Définir les couleurs pour chaque LED (ici, on met toutes les LED en rouge)
    for (int i = 0; i < LED_COUNT; i++) {
        ledstring.channel[0].leds[i] = 0x00FF0000;  // Couleur rouge (GRB)
    }

    // Rendre les couleurs sur les LEDs
    ws2811_render(&ledstring);

    // Garder les LEDs allumées pendant quelques secondes
    sleep(5);

    // Éteindre les LEDs
    for (int i = 0; i < LED_COUNT; i++) {
        ledstring.channel[0].leds[i] = 0x00000000;  // Éteindre toutes les LEDs
    }
    ws2811_render(&ledstring);

    // Nettoyage
    ws2811_fini(&ledstring);

    return 0;
}
