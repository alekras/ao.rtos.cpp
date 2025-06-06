#include "Include/iil.hpp"
#include "../../../Porting/ARM-Raspberry_Pi/Include/bcm_registers.hpp"
#include "../../../Porting/ARM-Raspberry_Pi/Include/gpio.hpp"

extern "C" int main();

extern "C" void uart_init();
extern "C" void uart_send(char);
extern "C" unsigned int uart_recv();

extern "C" void sendString(char*);
extern "C" void irq_vectors_setup();
extern "C" void led_setup();
extern "C" void arm_timer_setup(int);
extern "C" void enable_irq();

//extern "C" void dump_debug_init();
//extern "C" void dump_debug_message(char*);

//char out[120];
//FormatParser fp1;
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
//  dump_debug_message("Test 1\n\r");
//  dump_debug_message("Test 2\n\r");

//  do {} while(1);
//  fp.format(out, "(offset1) := %h.\n\r(offset2) := %h.\n\r(shift1) := %h.\n\r(mask) := %h.\n\r",
//      gpio47->offset1, gpio47->offset2, gpio47->shift1, gpio47->mask);
//  sendString(out);

//  fp1.format(out, "LINE STATUS := %h.\n\rINTERR STATUS := %h.\n\rCONTR REG := %h.\n\rSTATUS REG := %h.\n\r",
//    *pAUX_MU_LSR_REG, *pAUX_MU_IIR_REG, *pAUX_MU_CNTL_REG, *pAUX_MU_STAT_REG);
//  sendString(out);

  sendString("\n\rInitial Interactive Loader (c) krasnop@bellsouth.net\n\r");
  sendString("[build 0.1.10, 05/11/2025]\n\r");
  sendString("Commands:\n\r");
  sendString(" D<aaaa>,<cccc> - output memory content from <aaaa> to <aaaa> + <cccc>.\n\r");
  sendString(" D              - continue output memory content.\n\r");
  sendString(" M              - output debug dump content in text format.\n\r");
  sendString(" S              - represent memory content in text format.\n\r");
  sendString(" O              - represent memory content in bit.\n\r");
  sendString(" X              - represent memory content in hex.\n\r");
  sendString(" B              - set memory element as byte (wide = 1 byte).\n\r");
  sendString(" W              - set memory element as word (wide = 2 bytes).\n\r");
  sendString(" WW             - set memory element as double word (wide = 4 bytes).\n\r");
  sendString(" L              - upload hex file.\n\r");
  sendString(" H              - output help message.\n\r");
  sendString(" G<aaaa>        - run code at <aaaa> address.\n\r");

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
