#ifndef VIDEO_H
#define VIDEO_H

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include "types.h"
#include "model.h"

#define VIDEO_LOW_RES 0
#define VIDEO_HI_RES 1
#define OT_SIZE 4096

struct cd_File;

typedef struct video_Texture {
  RECT prect;
  RECT crect;
  u_short tpage;
  u_short clut;
  u_long mode;
  u_long caddr;
} video_Texture;

void video_init(int mode);
void video_init_frame();

void video_set_camera(VECTOR offset, SVECTOR angle);
void video_set_local(VECTOR offset, SVECTOR angle);
void video_reset_local();
void video_set_light_color(short r, short g, short b);

video_Texture video_load_texture(struct cd_File file);

void video_draw();
void video_draw_sprite(video_Texture texture, int x, int y);
void video_draw_poly4(POLY_F4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal);
void video_draw_poly3(POLY_GT3 *poly, SVECTOR vertices[3], CVECTOR colors[3], SVECTOR normals[3]);

#endif
