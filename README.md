# ao.rtos.cpp
AO-RTOS is a pure object-oriented small footprint real-time operating system for embedded devices. The project is using Active Object (or Actor) concurrency model and finite state machine (FSM) design template as implementation base.

# Project map:
 - [Kernel](Kernel/REARME.md) - implementation of RTOS kernel (source code and build tools)
 - [Library](Library/README.md) - tools and frameworks for system design based on the RTOS
 - [Porting](Porting/README.md) - parts of code specific for different microprocessor platforms and hardware
 - [Tools](Tools/README.md) - tools to support deploy and debug of microprocessor systems
 - [Applications](Applications/README.md) - examples of design of real implementations

Read more about the project principles in [Active Object real-time OS Web Site](http://krasnopolski.org/alpha/aortos.htm?ajax=github).

Porting to ARM processor (Raspbery Pi ver. 2) is implemented now.
