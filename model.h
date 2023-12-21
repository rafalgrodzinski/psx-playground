#ifndef MODEL_H
#define MODEL_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "types.h"

struct video_Texture;
struct cd_File;

typedef enum model_Poly_Type {
  model_Poly_Type_F4,
  model_Poly_Type_FT4
} model_Poly_Type;

typedef struct model_Model {
  model_Poly_Type poly_type;
  int polys_count;
  void *polys;
  POLY_FT4 *ft4_polys;
  POLY_GT3 *gt3_polys;
  SVECTOR **vertices;
  SVECTOR **normals;
  CVECTOR **colors;
} model_Model;

model_Model model_load_tmd(struct cd_File file, BOOL is_textured, struct video_Texture *texture);
model_Model model_create_cube(int size);
model_Model model_create_plane(int size, CVECTOR color, struct video_Texture *texture);

#endif
