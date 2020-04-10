#include <avr/io.h>

/*
  1 1 1 1 1 1 1 1 : setup (carrier and width follow)
  1|x x x x x x x : LSB out first, end before MSB zero
  0|x x x x x x|y : X + 1 y 
  0 0 0 0 0 0 0 0 : end of stream
*/

extern char delay;
extern unsigned int shift;
signed char buffer[100], *ptr;

namespace
{

signed char *rptr;

signed char next() {
  signed char c;
  if (!TCB0.CTRLA) return 0;	// forbid reading rptr
  if (!rptr) return 0;
  while ((c = *rptr) == -1) {
    TCA0.SINGLE.CTRLA = TCB0.CTRLA = 0;
    TCA0.SINGLE.CNT = TCB0.CNT = 0;
    TCB0.CCMPL = rptr[1] - 1;
    TCA0.SINGLE.PER = (int)rptr[2] * (int)rptr[1] - 1;
    TCB0.CTRLA = TCB_ENABLE_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
    rptr += 3;
  }
  rptr++;
  if (!c) {
    rptr = 0;
    if (!delay) if (!shift) TCA0.SINGLE.CTRLA = 0; TCB0.CTRLA = 0;
  }
  return c;
}

} // anonymous

bool getDuty() {
  static signed char x;
  signed char y = x | 1;
  if (y == -1) x = next();
  y = x;	// save bit
  if (y < 0) x >>= 1;
  else x -= 2;
  return y & 1;
}

bool transmitting_() { return TCB0.CTRLA; }

void transmit_() {
  *ptr++ = 126; *ptr++ = 126; *ptr = 0;
  rptr = buffer;
  TCB0.CTRLA = TCB_ENABLE_bm;	// let go next()
  TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
}

namespace user
{

void wait(int i) {
  while (transmitting_());
  ptr = buffer;
  while (i--) *ptr++ = 126;
  *ptr = 0;
  transmit_();
}

} // user
