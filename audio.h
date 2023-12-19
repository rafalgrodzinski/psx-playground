#ifndef AUDIO_H
#define AUDIO_H

#include <sys/types.h>
#include <libspu.h>
#include "types.h"

#define SPU_MALLOC_MAX 36

void audio_init();
void audio_load_voice(char *buffer, long size, u_long channel);
void audio_play_voice(u_long channel);
void audio_stop_voice(u_long channel);

#endif
