#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <yaml-cpp/yaml.h>
#include <cxxabi.h>
#include <Logger.hpp>

#define SPI_DELAY 0 // Maybe the wait-time between words (research required)
#define SPI_BPW 8 // Bit per word for the SPI transfer (basically how many bits are in one group)
#define LOG_AEREA_NEOSPI "NEOSPI"

using namespace std;

class Color { // Color class to define a color with one variable
    public:
      Color() {
        r = 0;
        g = 0;
        b = 0;
      }

      Color(uint8_t red, uint8_t green, uint8_t blue) {
        r = red;
        g = green;
        b = blue;
      }

      void setRed(uint8_t value) {
        r = value;
      }
      uint8_t getRed() {
        return r;
      }

      void setGreen(uint8_t value) {
        g = value;
      }
      uint8_t getGreen() {
        return g;
      }

      void setBlue(uint8_t value) {
        b = value;
      }
      uint8_t getBlue() {
        return b;
      }

      void changeBrightness(unsigned int brightness) {
        if (brightness > 100) brightness = 100;
        float multiplicationVal = float(brightness) / 100;
        r = float(r) * multiplicationVal;
        g = float(g) * multiplicationVal;
        b = float(b) * multiplicationVal;
      }
    private:
      uint8_t r; // Red color value
      uint8_t g; // Green color value
      uint8_t b; // Blue color value
};

class NeoSPI {
    public:
        bool begin(string spidev, long unsigned int numPixels, int _speed, YAML::Node loggerConfig);
        bool begin(string spidev, long unsigned int numPixels = 100, int _speed = 800);
        bool show();
        void fillStripRGB(uint8_t r, uint8_t g, uint8_t b); // Function to fill the strip using an RGB value
        bool setPixelRGB(uint8_t r, uint8_t g, uint8_t b, long unsigned int pos); // Function to set a single pixel using an RGB value (returns false if the pixel doesn't exist)
        void fillStrip(Color newColor); // Function to fill the strip
        bool setPixel(Color newColor, long unsigned int pos); // Function to set a single pixel (returns false if the pixel doesn't exist)
        bool setStripState(vector<Color> newStripState); // Set new contents for the entire strip
        long unsigned int getStripLength();
        void setStripLength(long unsigned int newStripLength); // WARNING: Clears the content of the strip
    private:
        bool writeBuff(vector<uint8_t> rawData);
        bool spiOpen(string dev);
        bool int2Bool(uint8_t input, int digit);

        vector<uint8_t> int2NeoPixel(uint8_t val);
        vector<uint8_t> RGB2BitBang(uint8_t r, uint8_t g, uint8_t b);
        
        int speed; //NeoPixel speed in KHz Newer NeoPixels are 800KHz older ones are 400KHz
        int SPIspeed;
        
        int spi_fd;
        vector<Color> stripState; // Basically a frame-buffer
};

