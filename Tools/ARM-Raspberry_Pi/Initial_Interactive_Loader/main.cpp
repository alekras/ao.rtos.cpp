#include "iil.hpp"
#include "bcm_registers.hpp"
#include "gpio.hpp"
#include "arm_debug_tools.hpp"

extern "C" int main();

extern "C" void uart_init();
extern "C" void uart_send(char);
extern "C" unsigned int uart_recv();

extern "C" void sendString(char*);
extern "C" void irq_vectors_setup();
extern "C" void led_setup();
extern "C" void arm_timer_setup(int);
extern "C" void enable_irq();

Gpio *gpio47, *gpio10, *gpio22;

//------------------------------------------------------------------------
int main() {
  unsigned char c;
  Gpio t(47), m(10), p(22);
  gpio47 = &t;
  gpio10 = &m;
  gpio22 = &p;

  irq_vectors_setup();
  arm_timer_setup(1000);
  led_setup();
  uart_init();
  enable_irq();

//  dump_debug_init();
//  dump_debug_message("Debug IIL\n\r");
//  dump_debug_message("Test 2\n\r");

//  fp1.format(out, "LINE STATUS := %h.\n\rINTERR STATUS := %h.\n\rCONTR REG := %h.\n\rSTATUS REG := %h.\n\r",
//    *pAUX_MU_LSR_REG, *pAUX_MU_IIR_REG, *pAUX_MU_CNTL_REG, *pAUX_MU_STAT_REG);
//  sendString(out);


  InitialInteractiveLoaderSM fsm;
  do {
    c = uart_recv();
    uart_send(c);
    if (fsm.dispatch((char*) &c) > 0 ) {
      break;
    }
  } while (1);

  return 0;
}
