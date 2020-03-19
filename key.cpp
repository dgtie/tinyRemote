#include <avr/io.h>
#include "lamos.h"

//////////////////////////////////////
// This file MUST be compiled with -O0

extern bool shift;

char delay;
void usart0();
bool getDuty();
void key(char);

static bool pa6(char) { return PORTA.IN & PIN6_bm; }

Service tca0([](){
  if (USART0.CTRLB) usart0();
  else {
    if (getDuty()) {
      TCB0.CCMPH = 30;
      PORTA.OUTSET = PIN7_bm;
    } else {
      TCB0.CCMPH = 0;
      if (!shift) PORTA.OUTCLR = PIN7_bm;
    }
    if (delay)
      if (!--delay) {
        if (!TCB0.CTRLA) TCA0.SINGLE.CTRLA = 0;
        PORTA.OUTSET = PIN3_bm | PIN2_bm | PIN1_bm;
        PORTB.OUTSET = PIN1_bm | PIN0_bm;
        if (!pa6(0)) key(0);
        else if (!pa6(PORTA.OUTCLR = PIN3_bm)) key(3);
        else if (!pa6(PORTA.OUTCLR = PIN2_bm)) key(2);
        else if (!pa6(PORTA.OUTCLR = PIN1_bm)) key(1);
        else if (!pa6(PORTB.OUTCLR = PIN1_bm)) key(5);
        else if (!pa6(PORTB.OUTCLR = PIN0_bm)) key(4);
        PORTA.OUTCLR = PIN3_bm | PIN2_bm | PIN1_bm;
        PORTB.OUTCLR = PIN1_bm | PIN0_bm;
        PORTA.INTFLAGS = PIN6_bm;
        PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
      }
  }
});
