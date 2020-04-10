#include <avr/interrupt.h>
#include "lamos.h"

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

namespace user {
void power(), mute(), vol_up(), vol_down(), ch_up(), ch_down();
void input(), k31(), k32(), k77(), k99();
}

extern char delay;
extern Service tca0;
void usart0(int), learn();

unsigned int shift;

void __attribute__((naked, section(".init3"))) init(void) {
  PORTA.DIRSET = PIN7_bm | PIN5_bm | PIN3_bm | PIN2_bm | PIN1_bm;
  PORTB.DIRSET = PIN2_bm | PIN1_bm | PIN0_bm;
  TCB0.CTRLB = TCB_CCMPEN_bm | TCB_CNTMODE_PWM8_gc;	// carrier
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
}

namespace
{

char ukey;
volatile char ikey;

int count;

Service porta([](){
  if (USART0.CTRLB) usart0(count);
  else {
    delay = 50;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
  }
});

} // anonymous

void key(char c) {
  ikey = ikey == c ? (c | 0x80) : c;
  SLPCTRL.CTRLA = 0;
}

int main(void) {
  TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;
  TCA0.SINGLE.PER = 1847;	// key bounce delay
  while (!TCA0.SINGLE.INTFLAGS);
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
  sei();
  if (PORTB.IN & PIN3_bm) learn();	// learn() won't return
  PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
  while ((SLPCTRL.CTRLA = SLPCTRL_SMODE_PDOWN_gc | SLPCTRL_SEN_bm)) {
    if (ukey != ikey) {
      ukey = ikey;
      using namespace user;
      if (shift) {
        shift = 0;
        switch (ukey & 7) {	// remove toggle bit
        case 0: k99(); break;		// ^^^^
        case 1: k77(); break;		// |  |
        case 2: k31(); break;		// 3  4
        case 3: mute(); break;		// 2  5
        case 4: power(); break;		// 1  0
        case 5: k32(); break;		// ----
        default:;
        }
      } else {
        switch (ukey & 7) {	// remove toggle bit
        case 0: vol_down(); break;	// ^^^^
        case 1: ch_down(); break;	// |  |
        case 2: ch_up(); break;		// 3  4
        case 3: input(); break;		// 2  5
        case 4: shift = 10000; break;	// 1  0
        case 5: vol_up(); break;	// ----
        default:;
        }
      }
    }
    if (!TCA0.SINGLE.CTRLA) asm("sleep");
  }
}

ISR(PORTA_PORT_vect) {
  count = TCA0.SINGLE.CNT;
  if (USART0.CTRLB) TCA0.SINGLE.CNT = 0;
  PORTA.INTFLAGS = PIN4_bm;
  if (PORTA.PIN6CTRL == PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc) {
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_INTDISABLE_gc; 
    SLPCTRL.CTRLA = 0;
  }
  _append(porta);
}

ISR(TCA0_OVF_vect) {
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
  _append(tca0);
}

/*
stty -F /dev/ttyUSB0 115200 raw
cat /dev/ttyUSB0
*/
