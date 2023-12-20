#ifndef SPRITE_H
#define SPRITE_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "types.h"

struct video_Texture;

typedef struct sprite_Sprite {
  //struct video_Texture texture;
  SPRT *sprite;
  DR_TPAGE *tpage_change;
  u_short tpage;
  u_long mode;
  RECT prect;
} sprite_Sprite;

sprite_Sprite sprite_load_sprite(struct video_Texture texture);

#endif
