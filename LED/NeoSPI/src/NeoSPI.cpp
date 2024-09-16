#include "include/NeoSPI.hpp"

bool NeoSPI::spiOpen(string dev) {
  try {
    if((spi_fd = open(dev.c_str(), O_RDWR)) < 0) { // Trying to open SPI device
      Logger::error("Error opening SPIDEV " + dev, LOG_AEREA_NEOSPI);
      return false;
    }
  } catch (...) {
    Logger::error("Failed to init SPIDEV " + dev + ": " + __cxxabiv1::__cxa_current_exception_type()->name(), LOG_AEREA_NEOSPI);
    return false;
  }

  return true; // Or just be happy :)
}

bool NeoSPI::writeBuff(vector<uint8_t> rawData) {
  try {
    struct spi_ioc_transfer spi; // A lot of complicated SPI stuff
    memset (&spi, 0, sizeof(spi)); // Reserving memory for SPI
    spi.tx_buf = reinterpret_cast<unsigned long>(rawData.data());    //(unsigned long)rawData; // Converts the array to a single long value
    spi.len = rawData.size(); // Sets the transfer size using the buffer size
    spi.delay_usecs = SPI_DELAY;
    spi.speed_hz = SPIspeed; // Sets the precise SPI speed
    spi.bits_per_word = SPI_BPW;
    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0) { // Gives all the information to the linux kernel for processing
      Logger::error("Error while trying to write to the SPIFD " + to_string(spi_fd), LOG_AEREA_NEOSPI);
      return false;
    }
    return true;
  } catch (...) {
    Logger::error("Error while trying to write to the SPIFD " + to_string(spi_fd) + ": " + __cxxabiv1::__cxa_current_exception_type()->name(), LOG_AEREA_NEOSPI);
    return false;
  }
}

bool NeoSPI::int2Bool(uint8_t input, int digit) {
   switch(digit) { // Split the uint8_t into individual bits in reverse direction
    case 7:
      return input & 0x01;
      break;
    case 6:
      return input & 0x02;
      break;
    case 5:
      return input & 0x04;
      break;
    case 4:
      return input & 0x08;
      break;
    case 3:
      return input & 0x10;
      break;
    case 2:
      return input & 0x20;
      break;
    case 1:
      return input & 0x40;
      break;
    case 0:
      return input & 0x80;
      break;
  }
  return false;
}

vector<uint8_t> NeoSPI::int2NeoPixel(uint8_t val) {
  int cnt = 0; // Initialize a counter with 7
  vector<uint8_t> color(8); // Initialize the array with 8 values for the 8 bits
  color = std::vector<uint8_t>(8, 0xC0); // Fill the LOW bytes in with the SPI BitBang bytes
  while (cnt < 8) { // Count through every bit of a byte
    if (int2Bool(val, cnt)) color.at(cnt) = 0xF8; // Fill the HIGH bytes in with the SPI BitBang bytes
    cnt++;
  }
  return color;
}

vector<uint8_t> NeoSPI::RGB2BitBang(uint8_t r, uint8_t g, uint8_t b) {
  vector<uint8_t> r_array = int2NeoPixel(r); // Get the SPI bytes for the red channel
  vector<uint8_t> g_array = int2NeoPixel(g); // Get the SPI bytes for the green channel
  vector<uint8_t> b_array = int2NeoPixel(b); // Get the SPI bytes for the blue channel

  int cnt = 0;
  vector<uint8_t> fullColor(24);
  while(cnt < 24) {
    if (cnt < 8) {
      fullColor.at(cnt) = r_array.at(cnt); // Append the red SPI bytes to the fullColor array
    } else if (cnt < 16) {
      fullColor.at(cnt) = g_array.at(cnt-8); // Append the green SPI bytes to the fullColor array
    } else if (cnt < 24) {
      fullColor.at(cnt) = b_array.at(cnt-16); // Append the blue SPI bytes to the fullColor array
    }
    cnt++;
  }
  return fullColor;
}

// Fill the strip with a specific color
void NeoSPI::fillStripRGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) {
  Color newColor(r, g, b); // Putting the new color into the Color class
  fillStrip(newColor);
}

// Set the color of a specific pixel
bool NeoSPI::setPixelRGB(uint8_t r, uint8_t g, uint8_t b, long unsigned int pos) {
  Color newColor(r, g, b);
  return setPixel(newColor, pos);
}

void NeoSPI::fillStrip(Color newColor) {
  stripState = std::vector<Color>(stripState.size(), newColor); // Saving the changes to the middle-man-buffer
}

// Set the color of a specific pixel
bool NeoSPI::setPixel(Color newColor, long unsigned int pos) {
  if (pos >= stripState.size()) return false; // Return false if the LED doesn't exist
  stripState.at(pos) = newColor; // Saving the changes to the middle-man-buffer
  return true; // If the length isn't larger than the amount of LEDs on the strip return true
}

bool NeoSPI::setStripState(vector<Color> newStripState) {
  if (newStripState.size() != stripState.size()) return false;
  stripState = newStripState;
  return true;
}

bool NeoSPI::show() {
  Color activeElement; // Create new Color value
  vector<uint8_t> rawPixel(24); // And a new raw SPI data value (number corresponds to bits per pixel)
  vector<uint8_t> rawData(stripState.size()*rawPixel.size()); // Init the vector used for storing the raw bitbang data
  long unsigned int cntTotal = 0; // Counter initialization
  long unsigned int cntColor = 0;
  long unsigned int cnt = 0; // Avoid a warning with long unsigned int instead of int
  while(cnt < stripState.size()) { // Looping through all of the LEDs
    activeElement = stripState.at(cnt); // Retrieve color from middle-man-buffer
    rawPixel = RGB2BitBang(activeElement.getRed(), activeElement.getGreen(), activeElement.getBlue()); // Convert that RGB value to the raw SPI bytes
    cntColor = 0;
    while (cntColor < rawPixel.size()) { // Count through all 24 bytes of the raw SPI color value for the current LED
        rawData.at(cntTotal) = rawPixel.at(cntColor); // Place that SPI value in the final array that (almost) represents the state of the physical LEDs
        cntColor++;
        cntTotal++;
    }
    cnt++;
  }
  return writeBuff(rawData); // Write the contents of the final raw SPI array to the LEDs
}

bool NeoSPI::begin(string spidev, long unsigned int numPixels, int _speed, YAML::Node loggerConfig) {
  return Logger::begin(loggerConfig) && begin(spidev, numPixels, _speed);
}

bool NeoSPI::begin(string spidev, long unsigned int numPixels, int _speed) {
  Logger::info("Initializing a NeoSPI instance on SPIDEV " + spidev + "...", LOG_AEREA_NEOSPI);
  speed = _speed;
  SPIspeed = speed*1024*8; // Calculate the SPI speed in bytes/s
  stripState.resize(numPixels); // Apply the size value to the arrays
  Logger::debug("Trying to initialize SPIDEV " + spidev, LOG_AEREA_NEOSPI);
  if (!spiOpen(spidev)) return false; // Open the SPI device
  Logger::debug("Successfully initialized SPIDEV " + spidev, LOG_AEREA_NEOSPI);
  usleep(100000); // Sleep for 100ms to ensure that the spidev is ready
  Logger::debug("Clearing all pixels in the strip...", LOG_AEREA_NEOSPI);
  fillStripRGB(0,0,0); // Clear the strip so there are no LEDs remaining on
  show(); // Commit the changes
  Logger::info("Successfully initialized NeoSPI instance on SPIDEV" + spidev, LOG_AEREA_NEOSPI);
  return true;
}

long unsigned int NeoSPI::getStripLength() {
  return stripState.size();
}

void NeoSPI::setStripLength(unsigned long newStripLength) {
  stripState.resize(newStripLength);
  stripState.clear();
}
