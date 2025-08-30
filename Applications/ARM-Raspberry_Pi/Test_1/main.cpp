#include "memory.hpp"
#include "arm_debug_tools.hpp"
#include "MyAO.hpp"
#include "GpioAO.hpp"
#include "ThermometerAO.hpp"
#include "UartAO.hpp"
#include "DebugAOScheduler.hpp"

//extern "C" int main();
extern "C" void enable_irq();
extern "C" void irq_vectors_setup();
extern "C" void arm_timer_setup(int);

extern MemoryManager* mm;

int main() {
  MemoryManager memMng;
  mm = &memMng;

  irq_vectors_setup();
  arm_timer_setup(250);
  dump_debug_init();
  dump_debug_f_message("Start AORTOS! Version= %d\r\n", 22);

  ISAObject::nestedLevel = 0;
// Objects allocation :
  Timer timer(0);
  UartAO mini_uart(1);
  GpioAO gpioAO(2);
  MyAO ao_1(4, 25);
  ThermometerAO therm(5);
  DebugAOScheduler scheduler;

// Timer setup:
  timer.addListener(&ao_1);
  timer.addListener(&gpioAO);
  timer.addListener(&scheduler);
  timer.addListener(&therm);
  scheduler.add(&timer);

// My AO setup :
  ao_1.addListener(&mini_uart);
  scheduler.add(&ao_1);

// Mini UART AO setup :
  mini_uart.addListener(&ao_1);
  scheduler.add(&mini_uart);

// GPIO setup
  scheduler.add(&gpioAO);

// ThermometerAO setup
  scheduler.add(&therm);

  scheduler.startOS();
//  we never come here
  while (1) {};
  return 0;
}
