#include <avr/interrupt.h>
#include "lamos.h"

extern unsigned char buffer[100], *ptr;

/*
  PA4: IR Transistor
  PA5: IR LED (TCB0/WO)
  PA6: Button Sense
  PA7: LED
  PB2, PB3: USART0 (TxD, RxD)
  PB0, PB1, PA1~3, GND: Button

  Default CLK_PER = 20 MHz / 6 = 3.333 MHz
  Only Px2 and Px6 are asynchronous
  Other PIN can't wake at RISING/FALLING

  to activate USART: power up with USART connected to PC
*/

namespace
{

unsigned char *rptr;
int last;

Service service([](){
  if (ptr == rptr) ptr = rptr = buffer;
  else {
    USART0.TXDATAL = *rptr++;
    USART0.CTRLA |= USART_DREIE_bm;
  }
});

} // anonymous

void learn(void) {
  ptr = rptr = buffer;
  TCA0.SINGLE.PER = 65535;	// 4.8 us x 65536 = 315 ms
  TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm;
  PORTA.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
  USART0.BAUD = 116;		// 115200 normal mode
  USART0.CTRLA = USART_RXCIE_bm;
  USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
  PORTA.OUTSET = PIN7_bm;
  while(1);
}

void usart0(int c) {
  if (!(PORTA.IN & PIN4_bm)) if (!last) return;
  *(int*)ptr = last = c; ptr += 2;
  USART0.CTRLA |= USART_DREIE_bm;
}

void usart0() {
  if (!last) return;
  *(int*)ptr = last = 0; ptr += 2;
  USART0.CTRLA |= USART_DREIE_bm;
}

ISR(USART0_RXC_vect) { USART0.RXDATAL; }

ISR(USART0_DRE_vect) {
  USART0.CTRLA &= ~USART_DREIE_bm;
  _append(service);
}

/*
stty -F /dev/ttyUSB0 115200 raw
cat /dev/ttyUSB0
*/
