#include "model.h"
#include "video.h"
#include "cd.h"

model_Model model_load_tmd(cd_File file, BOOL is_textured, video_Texture *texture) {
  model_Model model;
  TMD_PRIM tmd_prim;
  int i;

  model.polys_count = OpenTMD(file.buffer, 0);

  model.polys = (POLY_GT3*)malloc3(sizeof(POLY_GT3) * model.polys_count);
  model.vertices = (SVECTOR**)malloc3(sizeof(SVECTOR*) * model.polys_count);
  model.normals = (SVECTOR**)malloc3(sizeof(SVECTOR*) * model.polys_count);
  model.colors = (CVECTOR**)malloc3(sizeof(CVECTOR*) * model.polys_count);

  for (i=0; i<model.polys_count && ReadTMD(&tmd_prim); i++) {
    SetPolyGT3(&model.polys[i]);

    model.vertices[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 3);
    copyVector(&model.vertices[i][0], &tmd_prim.x0);
    copyVector(&model.vertices[i][1], &tmd_prim.x1);
    copyVector(&model.vertices[i][2], &tmd_prim.x2);
    
    model.normals[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 3);
    copyVector(&model.normals[i][0], &tmd_prim.n0);
    copyVector(&model.normals[i][1], &tmd_prim.n1);
    copyVector(&model.normals[i][2], &tmd_prim.n2);

    model.colors[i] = (CVECTOR*)malloc3(sizeof(CVECTOR) * 3);
    model.colors[i][0].cd = model.polys[i].code;
    model.colors[i][1].cd = model.polys[i].code;
    model.colors[i][2].cd = model.polys[i].code;

    if (is_textured) {
      setUV3(&model.polys[i], tmd_prim.u0, tmd_prim.v0,
        tmd_prim.u1, tmd_prim.v1,
        tmd_prim.u2, tmd_prim.v2);

      if (texture != NULL) {
        model.polys[i].tpage = texture->tpage;
        model.polys[i].clut = texture->clut;
      } else {
        model.polys[i].tpage = tmd_prim.tpage;
        model.polys[i].clut = tmd_prim.clut;
      }

      model.colors[i][0].r = 128;
      model.colors[i][0].g = 128;
      model.colors[i][0].b = 128;

      model.colors[i][1].r = 128;
      model.colors[i][1].g = 128;
      model.colors[i][1].b = 128;

      model.colors[i][2].r = 128;
      model.colors[i][2].g = 128;
      model.colors[i][2].b = 128;
    } else {
      model.colors[i][0].r = tmd_prim.r0;
      model.colors[i][0].g = tmd_prim.g0;
      model.colors[i][0].b = tmd_prim.b0;

      model.colors[i][1].r = tmd_prim.r1;
      model.colors[i][1].g = tmd_prim.g1;
      model.colors[i][1].b = tmd_prim.b1;

      model.colors[i][2].r = tmd_prim.r2;
      model.colors[i][2].g = tmd_prim.g2;
      model.colors[i][2].b = tmd_prim.b2;
    }
  }

  printf("Loaded model, polys count: %d\n", model.polys_count);

  return model;
}

model_Model model_create_cube(int size) {
  model_Model model;
  int i;
  SVECTOR p0 = { -size/2, -size/2, -size/2, 0 };
  SVECTOR p1 = { size/2, -size/2, -size/2, 0 };
  SVECTOR p2 = { size/2, size/2, -size/2, 0 };
  SVECTOR p3 = { -size/2, size/2, -size/2, 0 };

  SVECTOR p4 = { -size/2, -size/2, size/2, 0 };
  SVECTOR p5 = { size/2, -size/2, size/2, 0 };
  SVECTOR p6 = { size/2, size/2, size/2, 0 };
  SVECTOR p7 = { -size/2, size/2, size/2, 0 };

  SVECTOR n0 = { ONE, 0, 0, 0};
  SVECTOR n1 = { -ONE, 0, 0, 0};
  SVECTOR n2 = { 0, ONE, 0, 0};
  SVECTOR n3 = { 0, -ONE, 0, 0};
  SVECTOR n4 = { 0, 0, ONE, 0};
  SVECTOR n5 = { 0, 0, -ONE, 0};

  SVECTOR vertices[6][4] = {
    { p0, p1, p3, p2 },
    { p1, p5, p2, p6 },
    { p5, p4, p6, p7 },
    { p4, p0, p7, p3 },
    { p4, p5, p0, p1 },
    { p6, p7, p2, p3 }
  };

  SVECTOR *normals[6] = { &n5, &n0, &n4, &n1, &n3, &n2 };

  model.polys_count = 6;

  model.f4_polys = (POLY_F4*)malloc3(sizeof(POLY_F4) * model.polys_count);
  model.vertices = (SVECTOR**)malloc3(sizeof(SVECTOR*) * model.polys_count);
  model.normals = (SVECTOR**)malloc3(sizeof(SVECTOR*) * model.polys_count);
  model.colors = (CVECTOR**)malloc3(sizeof(CVECTOR*) * model.polys_count);

  for (i=0; i<model.polys_count; i++) {
    SetPolyF4(&model.f4_polys[i]);

    model.vertices[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 4);
    copyVector(&model.vertices[i][0], &vertices[i][0]);
    copyVector(&model.vertices[i][1], &vertices[i][1]);
    copyVector(&model.vertices[i][2], &vertices[i][2]);
    copyVector(&model.vertices[i][3], &vertices[i][3]);

    model.normals[i] = (SVECTOR*)malloc3(sizeof(SVECTOR));
    copyVector(&model.normals[i][0], &normals[i][0]);

    model.colors[i] = (CVECTOR*)malloc3(sizeof(CVECTOR));
    model.colors[i][0].cd = model.f4_polys[i].code;
    model.colors[i][0].r = 255;
    model.colors[i][0].g = 0;
    model.colors[i][0].b = 0;
  }

  printf("Created cube model of size %d\n", size);

  return model;
}
