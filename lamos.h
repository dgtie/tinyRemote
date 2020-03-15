#ifndef LAMOS_H
#define LAMOS_H

typedef void (*SERVICE)(void);

class Service {
public:
  Service(SERVICE);
private:
  SERVICE service;
  Service *next;
friend void _append(Service&);
friend class Queue;
};

void _append(Service&);		// only call it at end of ISR
void append(Service&);		// only call it in SERVICE

namespace user { void syscall(Service&); }

#endif /* LAMOS_H */
