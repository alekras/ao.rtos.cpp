
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

volatile unsigned int* pARM_TIMER_CTL = (unsigned int*) 0x3F00B408;
volatile unsigned int* pARM_TIMER_CNT = (unsigned int*) 0x3F00B420;

volatile unsigned int* pGPFSEL1 = (unsigned int*) 0x3F200004;
volatile unsigned int* pGPSET0  = (unsigned int*) 0x3F20001C;
volatile unsigned int* pGPCLR0  = (unsigned int*) 0x3F200028;
volatile unsigned int* pGPPUD   = (unsigned int*) 0x3F200094;
volatile unsigned int* pGPPUDCLK0 = (unsigned int*) 0x3F200098;

volatile unsigned int* pAUX_ENABLES    = (unsigned int*) 0x3F215004;
volatile unsigned int* pAUX_MU_IO_REG  = (unsigned int*) 0x3F215040;
volatile unsigned int* pAUX_MU_IER_REG = (unsigned int*) 0x3F215044;
volatile unsigned int* pAUX_MU_IIR_REG = (unsigned int*) 0x3F215048;
volatile unsigned int* pAUX_MU_LCR_REG = (unsigned int*) 0x3F21504C;
volatile unsigned int* pAUX_MU_MCR_REG = (unsigned int*) 0x3F215050;
volatile unsigned int* pAUX_MU_LSR_REG = (unsigned int*) 0x3F215054;
//#define AUX_MU_MSR_REG  0x3F215058
//#define AUX_MU_SCRATCH  0x3F21505C
volatile unsigned int* pAUX_MU_CNTL_REG = (unsigned int*) 0x3F215060;
//#define AUX_MU_STAT_REG 0x3F215064
volatile unsigned int* pAUX_MU_BAUD_REG = (unsigned int*) 0x3F215068;

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//------------------------------------------------------------------------
unsigned char uart_recv() {
  while (((*pAUX_MU_LSR_REG) & 0x01) == 0) {}
  return (unsigned char) ((*pAUX_MU_IO_REG) & 0xFF);
}
//------------------------------------------------------------------------
void uart_send(char c) {
  while (((*pAUX_MU_LSR_REG) & 0x20) == 0) {}
  (*pAUX_MU_IO_REG) = (unsigned int) c;
}
//------------------------------------------------------------------------
void sendString(char* buff) {
  while ((*buff) != 0) {
    uart_send(*(buff++));
  }
}
//------------------------------------------------------------------------
void uart_init() {
  unsigned int ra;

  (*pAUX_ENABLES) = 1;
  (*pAUX_MU_IER_REG) = 0;
  (*pAUX_MU_CNTL_REG) = 0;
  (*pAUX_MU_LCR_REG) = 3;
  (*pAUX_MU_MCR_REG) = 0;
  (*pAUX_MU_IER_REG) = 0; // ? one more
  (*pAUX_MU_IIR_REG) = 0xC6; // ? 0x06
  //((250,000,000/115200)/8)-1 = 270
  (*pAUX_MU_BAUD_REG) = 270;
  //((250,000,000/9600)/8)-1 = 3254
//  (*pAUX_MU_BAUD_REG) = 3254;
  ra = (*pGPFSEL1);
  ra &= ~ (7<<12); //gpio14
  ra |= 2<<12;    //alt5
  ra &= ~(7<<15); //gpio15
  ra |= 2<<15;    //alt5
  (*pGPFSEL1) = ra;
  (*pGPPUD) = 0;
  for (ra = 0; ra < 150; ra++) {};
  (*pGPPUDCLK0) = (1 << 14) | (1 << 15);
  for (ra = 0; ra < 150; ra++) {};
  (*pGPPUDCLK0) = 0;
  (*pAUX_MU_CNTL_REG) = 3;
}

