#include "input.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static struct termios old;

void
input_init(Input* in)
{
  memset(in->keys,     0, sizeof(in->keys));
  memset(in->pressed,  0, sizeof(in->pressed));
  memset(in->released, 0, sizeof(in->released));

  input_enable_raw_mode();
}

void
input_shutdown(void)
{
  input_disable_raw_mode();
}

void
input_enable_raw_mode (void)
{
  struct termios raw;

  tcgetattr (STDIN_FILENO, &old);
  raw = old;

  raw.c_lflag     &= ~(ICANON | ECHO);
  raw.c_cc[VMIN]  = 0;
  raw.c_cc[VTIME] = 0;

  tcsetattr (STDIN_FILENO, TCSANOW, &raw);
  fcntl (STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void
input_disable_raw_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &old);
}

void
input_update(Input* in)
{
  memset(in->pressed,  0, sizeof(in->pressed));
  memset(in->released, 0, sizeof(in->released));

  int c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    if ((c >= 0 && c < 256)) {
      if (!in->keys[c]) {
        in->keys[c] = true;
        in->pressed[c] = true;
      }
    }
  }

  // detect release keys
  for (int i = 0; i < 256; ++i) {
    if (in->keys[i]) {
      if (!in->pressed[i]) {
        in->keys[i] = false;
        in->released[i] = true;
      }
    }
  }
}
