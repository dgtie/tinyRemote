/*
  convert binary stream (from tvbg via ttyUSB0) to text string
  input is 16 bit binary representing duration of mark/space
  output is duration in micro-second

  $ g++ -o b2str b2str.cpp
  $ stty -F /dev/ttyUSB0 115200 raw
  $ cat /dev/ttyUSB0 | ./b2str
*/

#include <iostream>

using namespace std;

int main() {
  unsigned short m, s;
  float f;
  freopen(NULL, "rb", stdin);
  while (1) {
    fread(&m, 2, 2, stdin);
    m -= 17; f = 4.8 * m; m = (int)f;
    if (s > 0) {
      s += 17;
      f = 4.8 * s; s = (int)f;
    }
    cout << m << "  " << s << endl;
  }
  return 0;
}

/*
#!/usr/bin/python

import serial, struct

ser = serial.Serial('/dev/ttyUSB0', 115200)
while True:
  val = struct.unpack('<HH', ser.read(4))
  print val
*/
