# Project for testing compilation process and hardware.

The project is a set of few Active objects that are running independently and communicate to each others.
The application consists of:
1. MyAO active object displays system time and controls LED connected to PIO terminals.
2. UartAO ISA active object controls UART connection to developer computer and processes interrupts that came from UART controller
3. ThermometerAO ISA active object reads and displays temperature from DS18B20 temperature sensor, also processes interrupts from timer.
4. GpioAO ISA active object processes interrupt from GPIO terminal
5. DebugAOScheduler extends AOScheduler and displays status of AORTOS scheduler

## Build and run application
There are two ways to build and run test application.

1. **First way:**

In terminal make current direction {project root}//Applications/ARM_Raspberry_Pi/Test1

 ```bash
 $cd {project root}//Applications/ARM_Raspberry_Pi/Test1
 ```
 Run Make builder

 ```bash
 $make image
 ```
 File kernel.img is created in folder {project root}/_build/target/. Copy this file to microSD, insert to RPi board and reset the board. Serial port terminal connected to the board shows log like this:
 
 ![fig_001](../../../docs/figures/fig_001.jpg)
 ![fig_001](fig_001.jpg)

2. **Second way:**

### MyAO

### UartAO

### ThermometerAO

### GpioAO

### DebugAOScheduler