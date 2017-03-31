#ifndef __ATOM_H
#define __ATOM_H

#include <stdint.h>

typedef struct atom *Atom_T;

int Atom_length(const char *str);
const char* Atom_new(const char *str, int16_t len);
const char* Atom_string(const char *str);
const char* Atom_int(long n);

#endif

