#include "code.h"

namespace
{

void bit(char c, int n) {
  unsigned char b = 0b11111110;
  while (n--) {
    b <<= 2; if (c & 1) b++; else b |= 2;
    c >>= 1;
  }
  *ptr++ = b;
}

void byte(char c) {
  bit(c >> 5, 3);
  bit(c >> 2, 3);
  bit(c, 2);
}

} // anonymous

namespace user
{

void rc6(char a, char c, char mode) {
  static unsigned char t = 0b11101100;
  while (transmitting_());
  ptr = buffer;
  *ptr++ = -1; *ptr++ = 93; *ptr++ = 16;
  *ptr++ = 9; *ptr++ = 0b11001001;
  bit(mode, 0b100);
  *ptr++ = (t ^= 0b1111);
  byte(a); byte(c);
  transmit_();
}

} // user
