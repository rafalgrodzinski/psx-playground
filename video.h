#ifndef VIDEO_H
#define VIDEO_H

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include "types.h"

#define VIDEO_LOW_RES 0
#define VIDEO_HI_RES 1

#define FPS_LIMIT_30 2
#define FPS_LIMIT_60 0
#define FPS_LIMIT_NONE 1

struct sprite_Sprite;
struct model_Model;
struct cd_File;

typedef struct video_Texture {
  RECT prect;
  u_short tpage;
  u_short clut;
  u_long mode;
} video_Texture;

void video_init(int mode, CVECTOR background_color, int fps_limit, BOOL should_show_fps);
void video_init_frame();
void video_draw();

void video_set_camera(VECTOR offset, SVECTOR angle);
void video_set_local(VECTOR offset, SVECTOR angle);
void video_reset_local();
void video_set_light_color(short r, short g, short b);

video_Texture video_load_texture(struct cd_File file);

void video_draw_sprite(struct sprite_Sprite sprite, int x, int y);
void video_draw_model(struct model_Model model);
void video_draw_poly_f4(POLY_F4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal);
void video_draw_poly_ft4(POLY_FT4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal);
void video_draw_poly_gt3(POLY_GT3 *poly, SVECTOR vertices[3], CVECTOR colors[3], SVECTOR normals[3]);

#endif
