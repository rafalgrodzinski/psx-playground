#include "model.h"
#include "video.h"
#include "cd.h"

model_Model model_load_tmd(cd_File file, CVECTOR color, video_Texture *texture) {
  model_Model model;
  TMD_PRIM tmd_prim;
  void *poly;
  int i;

  model.polys_count = OpenTMD(file.buffer, 0);

  model.poly_type = model_Poly_Type_GT3;
  model.polys = malloc3(sizeof(POLY_GT3) * model.polys_count);
  model.vertices = (SVECTOR**)malloc3(sizeof(SVECTOR*) * model.polys_count);
  model.normals = (SVECTOR**)malloc3(sizeof(SVECTOR*) * model.polys_count);
  model.colors = (CVECTOR**)malloc3(sizeof(CVECTOR*) * model.polys_count);

  for (i=0; i<model.polys_count && ReadTMD(&tmd_prim); i++) {
    poly = &((POLY_GT3*)model.polys)[i];
    SetPolyGT3((POLY_GT3*)poly);

    model.vertices[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 3);
    copyVector(&model.vertices[i][0], &tmd_prim.x0);
    copyVector(&model.vertices[i][1], &tmd_prim.x1);
    copyVector(&model.vertices[i][2], &tmd_prim.x2);
    
    model.normals[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 3);
    copyVector(&model.normals[i][0], &tmd_prim.n0);
    copyVector(&model.normals[i][1], &tmd_prim.n1);
    copyVector(&model.normals[i][2], &tmd_prim.n2);

    model.colors[i] = (CVECTOR*)malloc3(sizeof(CVECTOR) * 3);
    model.colors[i][0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_GT3*)poly)->code };
    model.colors[i][1] = (CVECTOR) { color.r, color.g, color.b, ((POLY_GT3*)poly)->code };
    model.colors[i][2] = (CVECTOR) { color.r, color.g, color.b, ((POLY_GT3*)poly)->code };

    setUV3((POLY_GT3*)poly, 
      tmd_prim.u0, tmd_prim.v0,
      tmd_prim.u1, tmd_prim.v1,
      tmd_prim.u2, tmd_prim.v2);

    if (texture != NULL) {
      ((POLY_GT3*)poly)->tpage = texture->tpage;
      ((POLY_GT3*)poly)->clut = texture->clut;
    } else {
      ((POLY_GT3*)poly)->tpage = tmd_prim.tpage;
      ((POLY_GT3*)poly)->clut = tmd_prim.clut;
    }
  }

  printf("Loaded model, polys count: %d\n", model.polys_count);

  return model;
}

model_Model model_create_cube(int size, CVECTOR color, video_Texture *texture) {
  model_Model model;
  void *poly;
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

  if (texture != NULL) {
    model.poly_type = model_Poly_Type_FT4;
    model.polys = malloc3(sizeof(POLY_FT4) * model.polys_count);
  } else {
    model.poly_type = model_Poly_Type_F4;
    model.polys = malloc3(sizeof(POLY_F4) * model.polys_count);
  }
  model.vertices = (SVECTOR**)malloc3(sizeof(SVECTOR*) * model.polys_count);
  model.normals = (SVECTOR**)malloc3(sizeof(SVECTOR*) * model.polys_count);
  model.colors = (CVECTOR**)malloc3(sizeof(CVECTOR*) * model.polys_count);

  for (i=0; i<model.polys_count; i++) {
    model.vertices[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 4);
    copyVector(&model.vertices[i][0], &vertices[i][0]);
    copyVector(&model.vertices[i][1], &vertices[i][1]);
    copyVector(&model.vertices[i][2], &vertices[i][2]);
    copyVector(&model.vertices[i][3], &vertices[i][3]);

    model.normals[i] = (SVECTOR*)malloc3(sizeof(SVECTOR));
    copyVector(&model.normals[i][0], normals[i]);

    model.colors[i] = (CVECTOR*)malloc3(sizeof(CVECTOR));
    if (texture != NULL) {
      poly = &((POLY_FT4*)model.polys)[i];
      SetPolyFT4((POLY_FT4*)poly);
      model.colors[i][0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_FT4*)poly)->code };
      setUVWH((POLY_FT4*)poly, 0, 0, texture->prect.w, texture->prect.h);
      ((POLY_FT4*)poly)->tpage = texture->tpage;
      ((POLY_FT4*)poly)->clut = texture->clut;
      //SetShadeTex((POLY_FT4*)poly, 1);
    } else {
      poly = &((POLY_F4*)model.polys)[i];
      SetPolyF4((POLY_F4*)poly);
      model.colors[i][0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_F4*)poly)->code };
    }
  }

  printf("Created cube model of size %d\n", size);

  return model;
}

model_Model model_create_plane(int size, CVECTOR color, video_Texture *texture) {
  model_Model model;

  model.polys_count = 1;

  if (texture != NULL) {
    model.poly_type = model_Poly_Type_FT4;
    model.polys = malloc3(sizeof(POLY_FT4));
    SetPolyFT4(&model.polys[0]);
  } else {
    model.poly_type = model_Poly_Type_F4;
    model.polys = malloc3(sizeof(POLY_F4));
    SetPolyF4(&model.polys[0]);
  }
  model.vertices = (SVECTOR**)malloc(sizeof(SVECTOR*));
  model.normals = (SVECTOR**)malloc3(sizeof(SVECTOR*));
  model.colors = (CVECTOR**)malloc3(sizeof(CVECTOR*));

  model.vertices[0] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 4);
  model.vertices[0][0] = (SVECTOR) { -size/2, 0, size/2, 0};
  model.vertices[0][1] = (SVECTOR) { size/2, 0, size/2, 0};
  model.vertices[0][2] = (SVECTOR) { -size/2, 0, -size/2, 0};
  model.vertices[0][3] = (SVECTOR) { size/2, 0, -size/2, 0};

  model.normals[0] = (SVECTOR*)malloc3(sizeof(SVECTOR));
  model.normals[0][0] = (SVECTOR) { 0, -ONE, 0, 0};

  model.colors[0] = (CVECTOR*)malloc3(sizeof(CVECTOR));
  if (texture != NULL) {
    model.colors[0][0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_FT4*)&model.polys[0])->code };
    setUVWH((POLY_FT4*)&model.polys[0], 0, 0, texture->prect.w, texture->prect.h);
    ((POLY_FT4*)&model.polys[0])->tpage = texture->tpage;
    ((POLY_FT4*)&model.polys[0])->clut = texture->clut;
    //SetShadeTex(&model.polys[0], 1);
  } else {
    model.colors[0][0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_F4*)&model.polys[0])->code };
  }

  printf("Created plane model of size %d\n", size);

  return model;
}

model_Anim model_load_mime(cd_File file) {
}
