#include "memory.hpp"
#include "MyAO.hpp"

extern "C" int main();
extern "C" void enable_irq();

char out[200]; // @debug
FormatParser fp1; // @debug
extern MemoryManager* mm;
extern Gpio *gpio47, *gpio10, *gpio22;

int main() {
  Gpio t(47), m(10), p(22);
  gpio47 = &t;
  gpio10 = &m;
  gpio22 = &p;
  MemoryManager memMng;
  mm = &memMng;

  irq_vectors_setup();
  arm_timer_setup(1000);
  led_setup();
  dump_debug_init();
  fp1.format(out, "Start OS! Version= %d\r\n", 15);  // @debug
  dump_debug_message(out);  // @debug

  ISAObject::nestedLevel = 0;
// Objects allocation :
  Timer timer(0);
  UartAO mini_uart(1);
  MyAO ao_1(2, 25);
  AOScheduler scheduler;

// Timer setup:
//  timer.addListener(&timer);
  timer.addListener(&ao_1);
//  timer.addListener(&scheduler);
  scheduler.add(&timer);

// My AO setup :
  ao_1.addListener(&mini_uart);
  scheduler.add(&ao_1);

// Mini UART AO setup :
  mini_uart.addListener(&ao_1);
  scheduler.add(&mini_uart);

  scheduler.startOS();
//  we never come here
  while (1) {};
  return 0;
}
