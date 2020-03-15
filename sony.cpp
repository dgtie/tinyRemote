#include "code.h"

namespace
{

void byte(char b, int s) {
  for (int i = 0; i < s; i++) {
    *ptr++ = b & 1 ? 0b11110011 : 0b11111001;
    b >>= 1;
  }
}

void sony(char c, char a, char e, char m) {
  while (transmitting_());
  ptr = buffer;
  *ptr++ = -1; *ptr++ = 83; *ptr++ = 24;
  *ptr++ = 0b11001111;
  byte(c, 7);
  if (m != 15) byte(a, 5);
  a = e;
  if (m != 12) byte(a, 8);
  *ptr++ = 126; *ptr++ = 126;
  *ptr = 0;
  transmit_();
}

} // anonymous

namespace user
{

void sony12(char c, char a) { sony(c, a, 0, 12); }
void sony15(char c, char a) { sony(c, a, 0, 15); }
void sony20(char c, char a, char e) { sony(c, a, e, 20); }

}
