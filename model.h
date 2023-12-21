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
  POLY_F4 *f4_polys;
  SVECTOR **vertices;
  SVECTOR **normals;
  CVECTOR **colors;
} model_Model;

model_Model model_load_tmd(struct cd_File file, BOOL is_textured, struct video_Texture *texture);
model_Model model_create_cube(int size);

#endif
