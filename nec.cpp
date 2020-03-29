#include "code.h"

namespace
{

void b2(char c) {
  switch (c & 3) {
  case 0: *ptr++ = 0b11100101; break;
  case 1: *ptr++ = 0b10010001; break;
  case 2: *ptr++ = 0b10000101; break;
  default: *ptr++ = 0b11100001; *ptr++ = 0b11100001;
  }
}

void byte(char c) {
  b2(c); b2(c >>= 2); b2(c >>= 2); b2(c >> 2);
}

void nec(char a1, char a2, char c, int leading) {
  while (transmitting_());
  ptr = buffer;
  *ptr++ = -1; *ptr++ = 88; *ptr++ = 21;
  *ptr++ = (leading - 1) * 2 + 1; *ptr++ = 14;
  byte(a1); byte(a2); byte(c); byte(~c);
  *ptr++ = 0b11100001;
  transmit_();
}

} // anonymous

namespace user
{

void nec(char a, char c) { ::nec(a, ~a, c, 16); }
void nec_sc(short a, char c) { ::nec(a, a>>8, c, 16); }
void samsung(char a1, char a2, char c) { ::nec(a1, a2, c, 8); }

}
