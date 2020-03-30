/*
  input text stream of mark/space duration
  output ir remote code

  $ g++ -o decode decode.cpp
  $ cat /dev/ttyUSB0 | ./b2str | ./decode
  $ cat hex | ./pronto | ./decode
*/

#include <iostream>

using namespace std;

class NEC {
protected:
  virtual bool bit(int, int, int&) = 0;
  int lsb5(int *buffer) {
    int i = 0;
    if (!bit(buffer[0], 1, i)) return -1;
    if (!bit(buffer[2], 2, i)) return -1;
    if (!bit(buffer[4], 4, i)) return -1;
    if (!bit(buffer[6], 8, i)) return -1;
    if (!bit(buffer[8], 16, i)) return -1;
    return i;
  }
  int lsb7(int *buffer) {
    int i = 0;
    if (!bit(buffer[0], 1, i)) return -1;
    if (!bit(buffer[2], 2, i)) return -1;
    if (!bit(buffer[4], 4, i)) return -1;
    if (!bit(buffer[6], 8, i)) return -1;
    if (!bit(buffer[8], 16, i)) return -1;
    if (!bit(buffer[10], 32, i)) return -1;
    if (!bit(buffer[12], 64, i)) return -1;
    return i;
  }
  int lsb(int *buffer) {
    int i = 0;
    if (!bit(buffer[0], 1, i)) return -1;
    if (!bit(buffer[2], 2, i)) return -1;
    if (!bit(buffer[4], 4, i)) return -1;
    if (!bit(buffer[6], 8, i)) return -1;
    if (!bit(buffer[8], 16, i)) return -1;
    if (!bit(buffer[10], 32, i)) return -1;
    if (!bit(buffer[12], 64, i)) return -1;
    if (!bit(buffer[14], 128, i)) return -1;
    return i;
  }
};

class : NEC {
public:
  bool decode(int *buffer, int length) {
    if (length < 68) return false;
    for (int i = 2; i < 68; i += 2) if (buffer[i] != 1) return false;
    int a, b, c, d;
    a = lsb(&buffer[3]); if (a < 0) return false;
    b = lsb(&buffer[19]); if (b < 0) return false;
    c = lsb(&buffer[35]); if (c < 0) return false;
    d = lsb(&buffer[51]); if (d < 0) return false;
    if (buffer[0] == 16) cout << "NEC: ";
    else if (buffer[0] == 8) cout << "SAMSUNG: ";
    else cout << "NEC" << buffer[0] << "? ";
    cout << a << ", " << b << "(~" << (255 - b) << "), ";
    cout << c << ", " << d << "(~" << (255 - d) << ")";
    cout << endl;
    return true;
  }
private:
  bool bit(int u, int v, int &w) {
    if (u == 3) { w += v; return true; }
    if (u == 1) return true;
    return false;
  } 
} nec;

class : NEC {
public:
  bool decode(int *buffer, int length) {
    if (buffer[0] != 4) return false;
    int b;
    for (b = 1; b < length; b += 2) if (buffer[b] != 1) break;
    if ((b != 25) && (b != 31) && (b != 41)) return false;
    int c, a, e;
    c = lsb7(&buffer[2]); if (c < 0) return false;
    if (b != 31) {
        a = lsb5(&buffer[16]);
        if (a < 0) return false;
    }
    if (b == 31) {
        a = lsb(&buffer[16]);
        if (a < 0) return false;
        cout << "SONY15: " << c << ", " << a << endl;
        return true;
    }
    if (b == 41) {
        e = lsb(&buffer[26]);
        if (e < 0) return false;
        cout << "SONY20: " << c << ", " << a << ", " << e << endl;
        return true;
    }
    cout << "SONY12: " << c << ", " << a << endl;
    return true;
  }
private:
  bool bit(int u, int v, int &w) {
    if (u == 2) { w += v; return true; }
    if (u == 1) return true;
    return false;
  } 
} sony;

class : NEC {
public:
  bool decode(int *buffer, int length) {
    if (length < 32) return false;
    for (int i = 0; i < 32; i += 2) if (buffer[i] != 1) return false;
    int a, c;
    a = lsb5(&buffer[1]); if (a < 0) return false;
    c = lsb(&buffer[11]); if (c < 0) return false;
    cout << "SHARP: " << a << ", " << c << endl;
    return true;
  }
private:
  bool bit(int u, int v, int &w) {
    if ((u > 5) && (u < 9)) { w += v; return true; }
    if ((u > 1) && (u < 4)) return true;
    return false;
  } 
} sharp;

int *ptr;
bool mark;
bool nextbit() {
  if (!*ptr) return false;
  if (!--(*ptr)) { mark = !mark; ptr++; }
  return true;
}
int bit() {
  bool a = mark;
  if (!nextbit()) return -1;
  bool b = mark;
  if (!nextbit()) return -1;
  if (a && b) return -3;
  if ((!a) && (!b)) return -2;
  return b ? 0 : 1;
}
int mode() {
  int i, m;
  i = bit(); if (i < 0) return -1;
  m = i ? 4 : 0;
  i = bit(); if (i < 0) return -1;
  if (i) m += 2;
  i = bit(); if (i < 0) return -1;
  if (i) m++;
  return m;
}

int byte() {
  int i, b;
  i = bit(); if (i < 0) return -1;
  b = i ? 128 : 0;
  i = bit(); if (i < 0) return -1;
  if (i) b += 64;
  i = bit(); if (i < 0) return -1;
  if (i) b += 32;
  i = bit(); if (i < 0) return -1;
  if (i) b += 16;
  i = bit(); if (i < 0) return -1;
  if (i) b += 8;
  i = bit(); if (i < 0) return -1;
  if (i) b += 4;
  i = bit(); if (i < 0) return -1;
  if (i) b += 2;
  i = bit(); if (i < 0) return -1;
  if (i) b++;
  return b;
}

bool rc6(int *buffer) {
  if (buffer[1] != 2) return false;
  if (buffer[2] != 1) return false;	// start bit
  ptr = &buffer[3]; mark = false;
  if (!nextbit()) return false;
  int m, t0, t1, a, c; 
  m = mode(); if (m < 0) return false;
  t0 = bit(); t1 = bit();
  if ((t0 + t1) != -5) return false;
  a = byte(); if (a < 0) return false;
  c = byte(); if (c < 0) return false;
  cout << "RC6: " << m << ", " << a << ", " << c << endl;
  return true;
}

class RC5 {
public:
  bool decode(int* buffer) {
    if (*buffer != 1) return false;
    setBuffer(&buffer[1], false);
    if (readbit() != 1) return false;	// start bit
    if (readbit() < 0) return false;	// toggle bit
    int a = readbits(5);
    if (a < 0) return false;
    int c = readbits(6);
    if (c < 0) return false;
    if (*ptr) return false;
    cout << "RC5: " << a << ", " << c << endl;
    return true;
  }
private:
  int data, *ptr;
  bool mark;
  void setBuffer(int *p, bool m) { ptr = p; data = 0; mark = m; }
  bool readmark() {
    if (!*ptr) return false;
    bool m = mark;
    if (++data == *ptr) { data = 0; ++ptr; mark = !mark; }
    return m;
  }
  int readbit() {
    if (!*ptr) return -1;	// end of data
    bool a, b;
    a = readmark(); b = readmark();
    if (a && b) return -2;	// 11
    if (a || b) return b ? 1 : 0;
    else return -3;		// 00
  }
  int readbits(int i) {
    int b = 0, c;
    while (i--) {
      b <<= 1;
      c = readbit();
      if (c < 0) return -1;
      if (c) b++;
    }
    return b;
  }
} rc5;

int main() {
  int u, buffer[100], index, uc, ua;
  unsigned short s;
  while (!cin.eof()) {
    index = 0;
    u = ua = 65535; uc = 1;
    while (index < 100) {	// save to buffer and find out basic unit
      cin >> s;
      buffer[index++] = s;
      if (!s) break;
      if ((s < u + 70) && (s > u - 70)) { ua += s; ++uc; u = ua / uc; }
      else if (s < u) { u = ua = s; uc = 1; };
    }
    if (index == 1) break;
    index = 0;
    int u4 = u >> 4;
    while (buffer[index]) {	// normalization
      buffer[index] = (buffer[index] + u4) / u;
      if (!buffer[index]) buffer[index] = 1;
      index++;
    }
    index = 0;
    while (buffer[index++]);
    if (nec.decode(buffer, index));
    else if (sony.decode(buffer, index));
    else if (sharp.decode(buffer, index));
    else if (rc5.decode(buffer));
    else if (rc6(buffer));
    else {
      index = 0;
      while (1) {
        cout << buffer[index++] << " - " << buffer[index] << endl;
        if (!buffer[index++]) break;
      }
      cout << "I don't know!" << endl;
    }
  }
  return 0;
}
