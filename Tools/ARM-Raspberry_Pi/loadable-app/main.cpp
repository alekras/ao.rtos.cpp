#include "../../../Library/Display/Include/formatter.hpp"
#include "../../../Porting/ARM-Raspberry_Pi/Include/gpio.hpp"

extern "C" int main();

extern "C" void uart_init();
extern "C" void uart_send(char);
extern "C" unsigned int uart_recv();
extern "C" unsigned char read_rx_buffer();

extern "C" void sendString(char*);
extern "C" void irq_vectors_setup();
extern "C" void led_setup();
extern "C" void sys_timer_setup();
extern "C" void arm_timer_setup(int);
extern "C" void enable_irq();
extern "C" int * get_sp(void);
extern "C" void dump_debug_init();
extern "C" void dump_debug_message(char*);

char out[200];
FormatParser fp;
extern Gpio *gpio47, *gpio10, *gpio22;

//------------------------------------------------------------------------
int main() {
  int *stack0;
  Gpio t(47), m(10), p(22);
  gpio47 = &t;
  gpio10 = &m;
  gpio22 = &p;

  irq_vectors_setup();
  arm_timer_setup(500);
  led_setup();
  uart_init();
  dump_debug_init();
  enable_irq();

  sendString("\n\rLoadable application\n\r");
  sendString("--- Start ---\n\r");

//  dump_debug_message("Test 1\n\r");
//  dump_debug_message("Test 2\n\r");

  unsigned int i = 0x00ffffff, j = 0;

  do {
    do {} while ((i--) != 0);
    stack0 = get_sp();
    fp.format(out, "-> tick #%d SP=%h\n\r", j++, stack0);
    sendString(out);
    i = 0x00ffffff;
  } while (1);

  return 0;
}
