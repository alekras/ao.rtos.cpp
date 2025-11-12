# Initial Interactive Loader

## Intents
 0. Convenient way to restart system
 1. Check memory content
 2. Upload an application in hex format to RPi memory
 3. Start application from given address
 4. Support debug an application

## Prearrangement
 1. Connect RPi to serial port of host computer. There are a lot of tutorials how establish serial connection to RPi online.
  Check, please, 
      - [David Welch github account](https://github.com/dwelch67/raspberrypi)
      - [Learning operating system development using Linux kernel and Raspberry Pi](https://github.com/s-matyukevich/raspberry-pi-os/tree/master)
      - [How to Connect to the Serial Port on a Raspberry Pi using a USB Adapter or TTL Converter](https://practicingelectronics.wordpress.com/2018/04/22/serial-port-for-a-raspberry-pi-using-a-usb-to-serial-adapter/)
      - [USB to TTL Serial Cable - Debug / Console Cable for Raspberry Pi](https://www.adafruit.com/product/954)
 2. Make reset button
 3. Prepare microSD. Put files from microSD-image.zip to your SD.
 4. Install [arm gnu toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads). Change a Makefile (or build.xml for Ant builder) to reflect your installation folders.
 5. Install a serial port terminal. For Mac users I recomend CoolTerm is a simple serial port terminal application.
 
## Build and prepare to run
 1. Build image
     - cd ao.rtos.cpp/Tools/ARM-Raspberry_Pi/Initial_Interactive_Loader
     - make img
 2. Build microSD
     - copy file kernel.img to MicroSD
     - insert microCD to holder in RPi board and reset the target board.

## Run IIL
 After reset RPi (Interrupt power supply or using reset button) serial terminal shows image like this:

 ![fig_002](../../../docs/figures/fig_002.png)

 It is easy to explain IIL operations by examples. To display content of RPi memory use the command 'D'. Memory content represents in bytes:
 
 ![fig_003](../../../docs/figures/fig_003.png)
 
 To represent content in 2-byte words use command 'W'. Command 'WW' switchs memory view to 4-bytes words:
 
 ![fig_004](../../../docs/figures/fig_004.png)
 
 Command 'O' shows content of memory as a bit string. To switch to text view type command 'S', then return to hex byte view:
 
 ![fig_005](../../../docs/figures/fig_005.png)
 
 
 


## Functionality Explanation 