/*
  input pronto code stream
  output duration of mark/space in micro-second

  $ g++ -o pronto pronto.cpp
  $ cat hex | ./pronto
*/

#include <iostream>

using namespace std;

short s;
float k;

void output(int i) {
  static bool mark = true;
  s = (short)(k * i);
  if (mark) cout << s << " ";
  else cout << s << endl;
  mark = !mark;
}

int main() {
  int m, last = 0;
  while (!cin.eof()) {
    cin >> hex >> m;
    if (!cin) break;
    if (m) {
//      cout << (int)(k * last) << endl;
      output(last);
      last = m;
    } else {
      if (last) output(0);
      if (cin.eof()) break; cin >> hex >> last;
      k = 0.214246;
      k *= last;
      if (cin.eof()) break; cin >> hex >> last;
      if (cin.eof()) break; cin >> hex >> last;
      if (cin.eof()) break; cin >> hex >> last;
    }
  }
  output(0);
}
