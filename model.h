#ifndef MODEL_H
#define MODEL_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "types.h"

struct video_Texture;
struct cd_File;

typedef struct model_Model {
  int polys_count;
  POLY_GT3 *polys;
  SVECTOR **vertices;
  SVECTOR **normals;
  CVECTOR **colors;
} model_Model;

typedef struct model_Cube {
  POLY_F4 poly[6];
  SVECTOR vertices[6][4];
  SVECTOR normal[6];
  CVECTOR color[6];
} model_Cube;

model_Cube model_cube(short size, u_char r, u_char g, u_char b);
model_Model model_load_tmd(struct cd_File file, BOOL is_textured, struct video_Texture *texture);

#endif
