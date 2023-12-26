#ifndef CORE_H
#define CORE_H

#include <sys/types.h>
#include "libgte.h"
#include "types.h"

void core_init();
void core_init_all(int mode, CVECTOR background_color, int fps_limit, BOOL should_show_fps);

#endif
