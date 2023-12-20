#include "model.h"
#include "video.h"
#include "cd.h"

model_Cube model_cube(short size, u_char r, u_char g, u_char b) {
  model_Cube model;
  
  // 0
  SetPolyF4(&model.poly[0]);
  setVector(&model.vertices[0][0], -size, -size, -size);
  setVector(&model.vertices[0][1], +size, -size, -size);
  setVector(&model.vertices[0][2], -size, +size, -size);
  setVector(&model.vertices[0][3], +size, +size, -size);

  model.normal[0].vx = 0;
  model.normal[0].vy = 0;
  model.normal[0].vz = ONE;
  model.normal[0].pad = 0;

  model.color[0].cd = model.poly[0].code;
  model.color[0].r = r;
  model.color[0].g = g;
  model.color[0].b = b;

  // 1
  SetPolyF4(&model.poly[1]);
  setVector(&model.vertices[1][0], +size, -size, -size);
  setVector(&model.vertices[1][1], +size, -size, +size);
  setVector(&model.vertices[1][2], +size, +size, -size);
  setVector(&model.vertices[1][3], +size, +size, +size);

  model.normal[1].vx = -ONE;
  model.normal[1].vy = 0;
  model.normal[1].vz = 0;
  model.normal[1].pad = 0;

  model.color[1].cd = model.poly[1].code;
  model.color[1].r = r;
  model.color[1].g = g;
  model.color[1].b = b;

  // 2
  SetPolyF4(&model.poly[2]);
  setVector(&model.vertices[2][0], +size, -size, +size);
  setVector(&model.vertices[2][1], -size, -size, +size);
  setVector(&model.vertices[2][2], +size, +size, +size);
  setVector(&model.vertices[2][3], -size, +size, +size);

  model.normal[2].vx = 0;
  model.normal[2].vy = 0;
  model.normal[2].vz = -ONE;
  model.normal[2].pad = 0;

  model.color[2].cd = model.poly[2].code;
  model.color[2].r = r;
  model.color[2].g = g;
  model.color[2].b = b;

  // 3
  SetPolyF4(&model.poly[3]);
  setVector(&model.vertices[3][0], -size, -size, +size);
  setVector(&model.vertices[3][1], -size, -size, -size);
  setVector(&model.vertices[3][2], -size, +size, +size);
  setVector(&model.vertices[3][3], -size, +size, -size);

  model.normal[3].vx = ONE;
  model.normal[3].vy = 0;
  model.normal[3].vz = 0;
  model.normal[3].pad = 0;

  model.color[3].cd = model.poly[3].code;
  model.color[3].r = r;
  model.color[3].g = g;
  model.color[3].b = b;

  // 4
  SetPolyF4(&model.poly[4]);
  setVector(&model.vertices[4][0], -size, -size, +size);
  setVector(&model.vertices[4][1], +size, -size, +size);
  setVector(&model.vertices[4][2], -size, -size, -size);
  setVector(&model.vertices[4][3], +size, -size, -size);

  model.normal[4].vx = 0;
  model.normal[4].vy = ONE;
  model.normal[4].vz = 0;
  model.normal[4].pad = 0;

  model.color[4].cd = model.poly[4].code;
  model.color[4].r = r;
  model.color[4].g = g;
  model.color[4].b = b;

  // 5
  SetPolyF4(&model.poly[5]);
  setVector(&model.vertices[5][0], -size, +size, -size);
  setVector(&model.vertices[5][1], +size, +size, -size);
  setVector(&model.vertices[5][2], -size, +size, +size);
  setVector(&model.vertices[5][3], +size, +size, +size);

  model.normal[5].vx = 0;
  model.normal[5].vy = -ONE;
  model.normal[5].vz = 0;
  model.normal[5].pad = 0;

  model.color[5].cd = model.poly[5].code;
  model.color[5].r = r;
  model.color[5].g = g;
  model.color[5].b = b;

  return model;
}

model_Model model_load_tmd(cd_File file, BOOL is_textured, video_Texture *texture) {
  model_Model model;

  long polys_count;
  long number = 0;
  TMD_PRIM tmd_prim;
  int i;

  polys_count = OpenTMD(file.buffer, 0);

  model.count = polys_count;
  model.polys = (POLY_GT3 *)malloc3(sizeof(POLY_GT3) * polys_count);
  model.vertices = (SVECTOR**)malloc3(sizeof(SVECTOR*) * polys_count);
  model.normals = (SVECTOR**)malloc3(sizeof(SVECTOR*) * polys_count);
  model.colors = (CVECTOR**)malloc3(sizeof(CVECTOR*) * polys_count);

  for (i=0; i<polys_count && ReadTMD(&tmd_prim); i++) {
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
      setUV3(&model.polys[i], tmd_prim.u0, tmd_prim.v0, tmd_prim.u1, tmd_prim.v1, tmd_prim.u2, tmd_prim.v2);
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

    //SetSemiTrans(&model.polys[i], 0);
  }

  printf("Loaded model, poly count: %d\n", polys_count);

  return model;
}

