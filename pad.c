#include "pad.h"

char buffer[2][34];

void pad_init() {
  InitPAD(buffer[0], 34, buffer[1], 34);
  buffer[0][0] = buffer[0][1] = 0xff;
  buffer[1][0] = buffer[1][1] = 0xff;
  StartPAD();
}

BOOL pad_is_connected(int port) {
  return buffer[port][0] == 0;
}

BOOL pad_is_pressed(int port, int button) {
  int status = buffer[port][2];
  status |= buffer[port][3] << 8;
  return !(status & button);
}

