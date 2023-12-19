#ifndef PAD_H
#define PAD_H

#include <sys/types.h>
#include <libapi.h>
#include "types.h"

#define PAD_1 0
#define PAD_2 1

#define PAD_UP 16
#define PAD_DOWN 64
#define PAD_LEFT 128
#define PAD_RIGHT 32

#define PAD_CROSS 16384
#define PAD_CIRCLE 8192
#define PAD_TRIANGLE 4096
#define PAD_SQUARE 32768

#define PAD_START 8
#define PAD_SELECT 1

#define PAD_L1 1024
#define PAD_L2 256
#define PAD_R1 2048
#define PAD_R2 512

void pad_init();
BOOL pad_is_connected(int port);
BOOL pad_is_pressed(int port, int button);

#endif
