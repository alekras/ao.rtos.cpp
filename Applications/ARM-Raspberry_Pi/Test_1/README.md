# Project for testing compilation process and hardware.

The project is a set of few Active objects that are running independently and communicate to each others.
The application consists of:
1. MyAO active object displays system time and controls LED connected to PIO terminals.
2. UartAO ISA active object controls UART connection to developer computer and processes interrupts that came from UART controller
3. ThermometerAO ISA active object reads and displays temperature from DS18B20 temperature sensor, also processes interrupts from timer.
4. GpioAO ISA active object processes interrupt from GPIO terminal
5. DebugAOScheduler extends AOScheduler displays status of AORTOS scheduler