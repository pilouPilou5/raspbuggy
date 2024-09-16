
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <NeoSPI.hpp>


void ledInit(int nb_led,int spi_out ) ;
void pixelSet(int g,int r,int b,int i);
void blink(uint8_t g, uint8_t r, uint8_t b, int interval, int repetitions);
static uint8_t gamma8(uint8_t x);
void int2RGB(uint16_t n, uint32_t c);
uint32_t ColorHSV(uint16_t hue, uint8_t sat , uint8_t val ) ;
uint32_t gamma32(uint32_t x);
void rainbow(uint16_t first_hue , int8_t reps ,
               uint8_t saturation , uint8_t brightness ,
               bool gammify ) ;
void rainbow3(int wait);
void colorWipe(uint8_t r, uint8_t g, uint8_t b, int wait) ;
void theaterChaseRainbow(int wait);
void theaterChase(uint8_t rc, uint8_t gc, uint8_t bc, int wait);
static uint32_t RGB2int(uint8_t r, uint8_t g, uint8_t b);
