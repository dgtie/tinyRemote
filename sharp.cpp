#include "code.h"

namespace
{

void word(short s) {
  for (int i = 0; i < 16; i++) {
    *ptr++ = 0b11100001;
    if (s & 1) *ptr++ = 0b11100000;
    s >>= 1;
  }
}

} // anonymous

namespace user
{

void sharp(char a, char c) {
  short s = 0x2000 | (c << 5) | a;
  while (transmitting_());
  ptr = buffer;
  *ptr++ = -1; *ptr++ = 78; *ptr++ = 10;
  word(s);
  *ptr++ = 126; *ptr++ = 126;
  word(s ^ 0x7fe0);
  *ptr++ = 126; *ptr++ = 126;
  *ptr = 0;
  transmit_();
}

} // user
