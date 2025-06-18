## Initial Interactive Loader

# Intents
 0. Convenient way to restart system
 1. Check memory content
 2. Upload an application in hex format to RPi memory
 3. Start application from given address
 4. Support debug an application

# Prearrangement
 1. Connect RPi to serial port of computer. There are a lot of tutorials how establish serial connection to RPi online.
  Check, please, 
      - [David Welch github account](https://github.com/dwelch67/raspberrypi)
      - [How to Connect to the Serial Port on a Raspberry Pi using a USB Adapter or TTL Converter](https://practicingelectronics.wordpress.com/2018/04/22/serial-port-for-a-raspberry-pi-using-a-usb-to-serial-adapter/)
  [USB to TTL Serial Cable - Debug / Console Cable for Raspberry Pi](https://www.adafruit.com/product/954)
 2. Make reset button
 3. Prepare microSD
 
# Build and prepare to run
 1. Build image
     - cd ao.rtos.cpp/Tools/ARM-Raspberry_Pi/Initial_Interactive_Loader
     - make
  2. Build microSD
     - copy file kernel.img to MicroSD
     - insert microCD to holder in RPi and reset target device.

# Functionality Explanation 