#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

typedef struct {
  bool keys     [256];
  bool pressed  [256];
  bool released [256];
} Input;

void
input_init(Input* in);

void
input_shutdown(void);

void
input_enable_raw_mode(void);

void
input_disable_raw_mode(void);

void
input_update(Input* in);

#endif /* INPUT_H */
