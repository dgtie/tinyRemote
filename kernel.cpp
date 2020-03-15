#include <avr/interrupt.h>
#include "lamos.h"

static class Queue {
public:
  Queue(): busy(false), last(0) {}
  bool busy;
  Service &push(Service &s) {
    if (last) s.next = last->next; else last = &s;
    last->next = &s;
    return s;
  }
  void append(Service &s) { last = &push(s); }
  Service *pop() {
    Service *s = 0;
    busy = true;
    if (last) {
      s = last->next;
      last->next = s->next;
      s->next = 0;
      if (!last->next) last = 0;
    }
    busy = false;
    return s;
  }
  bool empty() { return !last; }
private:
  Service *last;
} kernel, isr;

void append(Service &s) { kernel.append(s); }

void _append(Service &s) {	// only call it at end of ISR
  static bool k_level;
  if (isr.busy) kernel.push(s);
  else isr.append(s);
  if (!k_level) {
    k_level = true;
    while (!isr.empty()) {
      sei();
      Service *p;
      while ((p = isr.pop()) ? p : (p = kernel.pop())) p->service();
      cli();
    }
    k_level = false;
  }
}

Service::Service(SERVICE s): service(s), next(0) {}

namespace user { void syscall(Service &s) { cli(); _append(s); sei(); } }
