# NeoSPI
A simplistic C++ SPI NeoPixel driver that works on any linux device with SPI

- to compile run `meson setup <builddir>` and afterwards `ninja -C <builddir>`
- run example: `<builddir>/example/standarttest /dev/spidev0.0`

- Troubleshooting
	- Currently this library doesn't work on a Raspberry Pi 4 (tested and working on the Raspberry Pi 5 currently unknown if it works on the Raspberry Pi 3 and earlier)
  	- If you are using a Raspberry Pi 4 or similar you need to inhibit the downclocking of the CPU otherwise the LEDs will be white
  	- If your LEDs aren't RGB LEDs but they are a different order or they have an additional white component then they wont work with this library but if you open an issue I will try to implement them (I haven't implemented them because I don't own any)
  	- If you see any flickering or lower brightness than expected then please check your powersupply before opening a issue
  	- If the LEDs are currupted check that the spidev on your device is 6.4MHz for the 800KHz neopixels or 3.2MHz for the 400KHz models
- If you want to drive more than 170 LEDs (for RGB and similar less with RGBW) it is necesary to increase the SPI buffer size using `spidev.bufsiz=65536` in the kernel command line
- Credits
	- SPI C/C++ code: https://forums.raspberrypi.com/viewtopic.php?t=304828#p1824960
	- Example code: https://github.com/adafruit/Adafruit_NeoPixel
