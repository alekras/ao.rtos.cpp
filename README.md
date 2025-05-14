# ao.rtos.cpp
AO-RTOS is a pure object-oriented small footprint real-time operating system for embedded devices. The project is using Active Object (or Actor) concurrency model and finite state machine (FSM) design template as implementation base.

# Project map:
- [Kernel](Kernel/README.md) - implementation of RTOS kernel (source code and build tools)
- [Library](Library/README.md) - tools and frameworks for system design based on the RTOS
- [Porting](Porting/README.md) - parts of code specific for different microprocessor platforms and hardware
- [Tools](Tools/README.md) - tools to support deploy and debug of microprocessor systems
    - [Initial Interactive Loader](Tools/ARM-Raspberry_Pi/Initial_Interactive_Loader/README.md) - simple monitor connected to computer for uploading application to RPi memory and debugging
- [Applications](Applications/README.md) - examples of design of real implementations
    - [Test task](Applications/ARM-Raspberry_Pi/Test_1/README.md) - test application for ARM microprocessor (Paspberry Pi 2)

Read more about the project principles in [Active Object real-time OS Web Site](http://krasnopolski.org/alpha/aortos.htm?ajax=github).

Porting to ARM processor (Raspbery Pi ver. 2) is implemented now.
