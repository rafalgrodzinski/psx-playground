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
struct model_Object;
struct model_Anim;
struct cd_File;

typedef struct video_Image {
  RECT prect;
  u_short tpage;
  u_short clut;
  u_long mode;
} video_Image;

typedef struct video_Texture {
  int images_count;
  video_Image *images;
} video_Texture;

void video_init(int mode, CVECTOR background_color, int fps_limit, BOOL should_show_fps);
void video_init_frame();
void video_draw();

void video_set_camera(VECTOR offset, SVECTOR angle);
void video_set_1_light(SVECTOR color, SVECTOR angle, CVECTOR ambient_color);
void video_set_2_lights(SVECTOR color0, SVECTOR angle0, SVECTOR color1, SVECTOR angle1, CVECTOR ambient_color);
void video_set_3_lights(SVECTOR color0, SVECTOR angle0, SVECTOR color1, SVECTOR angle1, SVECTOR color2, SVECTOR angle2, CVECTOR ambient_color);

video_Texture video_load_texture(struct cd_File file);

void video_draw_sprite(struct sprite_Sprite sprite, int x, int y);
void video_draw_model(struct model_Model model);
void video_draw_object(struct model_Object object);

void video_draw_poly_f3(POLY_F3 *poly, SVECTOR vertices[3], CVECTOR color, SVECTOR normal);
void video_draw_poly_f4(POLY_F4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal);

void video_draw_poly_ft3(POLY_FT3 *poly, SVECTOR vertices[3], CVECTOR color, SVECTOR normal);
void video_draw_poly_ft4(POLY_FT4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal);

void video_draw_poly_g3(POLY_G3 *poly, SVECTOR vertices[3], CVECTOR colors[3], SVECTOR normals[3]);
void video_draw_poly_g4(POLY_G4 *poly, SVECTOR vertices[4], CVECTOR colors[4], SVECTOR normals[4]);

void video_draw_poly_gt3(POLY_GT3 *poly, SVECTOR vertices[3], CVECTOR colors[3], SVECTOR normals[3]);
void video_draw_poly_gt4(POLY_GT4 *poly, SVECTOR vertices[4], CVECTOR colors[4], SVECTOR normals[4]);

void video_animate_model(struct model_Model model, struct model_Anim anim, int n);

#endif
