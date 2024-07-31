#ifndef MODEL_H
#define MODEL_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "types.h"

struct video_Texture;
struct cd_File;

typedef enum model_Poly_Type {
  model_Poly_Type_F3 = 100,
  model_Poly_Type_F4,
  model_Poly_Type_FT3,
  model_Poly_Type_FT4,
  model_Poly_Type_G3,
  model_Poly_Type_G4,
  model_Poly_Type_GT3,
  model_Poly_Type_GT4
} model_Poly_Type;

typedef struct model_Poly {
	model_Poly_Type type;
	void *gpu_poly;
	u_short vertice_offsets[4];
	SVECTOR normals[4];
	CVECTOR colors[4];
} model_Poly;

typedef struct model_Mesh {
	int polys_count;
	model_Poly *polys;
  SVECTOR *vertices;
} model_Mesh;

typedef struct model_Model {
  int meshes_count;
  model_Mesh *meshes;
  //SVECTOR *vertices;
} model_Model;

typedef struct model_Anim {
  int count; 
  //SVECTOR *vertices;
  SVECTOR *original_vertices;
  SVECTOR *diffs;
} model_Anim;

typedef struct model_Object {
  model_Model model;
  VECTOR offset;
  SVECTOR angle;
  VECTOR scale;
} model_Object;

model_Model model_load_tmd(struct cd_File file, struct video_Texture *texture);
model_Model model_create_cube(int size, CVECTOR color, struct video_Texture *texture);
model_Model model_create_plane(int size, CVECTOR color, struct video_Texture *texture);
//model_Anim model_load_mime(struct cd_File file, model_Model model);

#endif
