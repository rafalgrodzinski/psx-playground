#include "model.h"
#include "video.h"
#include "cd.h"

model_Model model_load_tmd(cd_File file, video_Texture *texture) {
  model_Model model;
  TMD_PRIM tmd_prim;
  int i, j;
  u_long *ptr = (u_long*)file.buffer;
  u_char is_address_absolute;
  
  is_address_absolute = *ptr++ & 0x1; // id
  ptr++; // flags
  model.meshes_count = *ptr++;
  model.meshes = malloc3(sizeof(model_Mesh) * model.meshes_count);
  
  //printf("vert_top: 0x%x\n", ptr++);
  //printf("n_vert: %d\n", *ptr++);

  printf("Loading %d meshes\n", model.meshes_count);
  for (i=0; i<model.meshes_count; i++) {
    model_Mesh *mesh = &model.meshes[i];
    mesh->polys_count = OpenTMD(file.buffer, i);
    mesh->polys = malloc3(sizeof(model_Poly) * mesh->polys_count);
    
    printf("Loading mesh %d with %d polys\n", i, mesh->polys_count);

    for (j=0; j<mesh->polys_count && ReadTMD(&tmd_prim); j++) {
      void *poly;
      u_char mode = (tmd_prim.id >> 24) & 0xff;
      u_char flag = (tmd_prim.id >> 16) & 0xff;
    
      u_char code = (mode >> 5) & 0x7;
      u_char is_gouard_shaded = (mode >> 4) & 0x1;
      u_char is_quad = (mode >> 3) & 0x1;
      u_char is_textured = (mode >> 2) & 0x1;
      u_char is_translucent = (mode >> 1) & 0x1;
      u_char is_texture_lit = mode & 0x1;
      u_char is_multi_color = (flag >> 2) & 0x1;
      u_char is_double_faced = (flag >> 1) & 0x1;
      u_char is_lit = (flag >> 0) & 0x1;

      if (j==0) {
        // This doesn't seem right, how is the data loaded?
        mesh->vertices = tmd_prim.v_ofs;
      }
	
      //printf("code: %d, gouard: %d, quad: %d, texture: %d, trans: %d, tex lit: %d, size: %d, multi col: %d, double face: %d, lit: %d\n",
      //code, is_gouard_shaded, is_quad, is_textured, is_translucent, is_texture_lit, sizeof(u_char), is_multi_color, is_double_faced, is_lit);
	
      // F3
      if (!is_quad && !is_textured && !is_gouard_shaded) {
        //printf("F3\n");
        mesh->polys[j].type = model_Poly_Type_F3;
        mesh->polys[j].gpu_poly = malloc3(sizeof(POLY_F3));
        SetPolyF3((POLY_F3*)mesh->polys[j].gpu_poly);
      // F4
      } else if (is_quad && !is_textured && !is_gouard_shaded) {
        //printf("F4\n");
        mesh->polys[j].type = model_Poly_Type_F4;
        mesh->polys[j].gpu_poly = malloc3(sizeof(POLY_F4));
        SetPolyF4((POLY_F4*)mesh->polys[j].gpu_poly);
      // FT3
      } else if (!is_quad && is_textured && !is_gouard_shaded) {
        printf("FT3\n");
        continue;
        mesh->polys[j].type = model_Poly_Type_FT3;
        //model.polys[i].gpu_poly = malloc3(sizeof(POLY_FT3));
        //SetPolyFT3((POLY_FT3*)model.polys[i].gpu_poly);
      // FT4
      } else if (is_quad && is_textured && !is_gouard_shaded) {
        printf("FT4\n");
        continue;
        mesh->polys[j].type = model_Poly_Type_FT4;
      // G3
      } else if (!is_quad && !is_textured && is_gouard_shaded) {
        //printf("G3\n");
        mesh->polys[j].type = model_Poly_Type_G3;
        mesh->polys[j].gpu_poly = malloc3(sizeof(POLY_G3));
        SetPolyG3((POLY_G3*)mesh->polys[j].gpu_poly);
      // G4
      } else if (is_quad && !is_textured && is_gouard_shaded) {
        //printf("G4\n");
        mesh->polys[j].type = model_Poly_Type_G4;
        mesh->polys[j].gpu_poly = malloc3(sizeof(POLY_G4));
        SetPolyG4((POLY_G4*)mesh->polys[j].gpu_poly);
      // GT3
      } else if (!is_quad && is_textured && is_gouard_shaded) {
        printf("GT3\n");
        continue;
        mesh->polys[j].type = model_Poly_Type_GT3;
      // GT4
      } else if (is_quad && is_textured && is_gouard_shaded) {
        printf("GT4\n");
        continue;
        mesh->polys[j].type = model_Poly_Type_GT4;
      // Others are unsupported
      } else {
        printf("Other\n");
        continue;
      }

    /*model.vertices[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 3);
    copyVector(&model.vertices[i][0], &tmd_prim.x0);
    copyVector(&model.vertices[i][1], &tmd_prim.x1);
    copyVector(&model.vertices[i][2], &tmd_prim.x2);*/
	//printf("x0: %d, y0: %d, z0: %d\n", tmd_prim.x0.vx, tmd_prim.x0.vy, tmd_prim.x0.vz);
	//printf("x1: %d, y1: %d, z1: %d\n", tmd_prim.x1.vx, tmd_prim.x1.vy, tmd_prim.x1.vz);
	//printf("x2: %d, y2: %d, z2: %d\n", tmd_prim.x2.vx, tmd_prim.x2.vy, tmd_prim.x2.vz);

    //model.raw_vertice_offsets[i] = (u_short*)malloc3(sizeof(u_short) * 3);
    //model.raw_vertice_offsets[i][0] = tmd_prim.vert0;
    //model.raw_vertice_offsets[i][1] = tmd_prim.vert1;
    //model.raw_vertice_offsets[i][2] = tmd_prim.vert2;
      mesh->polys[j].vertice_offsets[0] = tmd_prim.vert0;
      mesh->polys[j].vertice_offsets[1] = tmd_prim.vert1;
      mesh->polys[j].vertice_offsets[2] = tmd_prim.vert2;
      mesh->polys[j].vertice_offsets[3] = tmd_prim.vert3;
    //printf("v0: %d, v1: %d, v2: %d\n", tmd_prim.vert0, tmd_prim.vert1, tmd_prim.vert2);
    
    //model.normals[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 3);
    //copyVector(&model.normals[i][0], &tmd_prim.n0);
    /*copyVector(&model.normals[i][1], &tmd_prim.n0);
    copyVector(&model.normals[i][2], &tmd_prim.n0);*/
      copyVector(&mesh->polys[j].normals[0], &tmd_prim.n0);
      copyVector(&mesh->polys[j].normals[1], &tmd_prim.n1);
      copyVector(&mesh->polys[j].normals[2], &tmd_prim.n2);
      copyVector(&mesh->polys[j].normals[3], &tmd_prim.n3);

    //model.colors[i] = (CVECTOR*)malloc3(sizeof(CVECTOR) * 3);
    //model.colors[i][0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_GT3*)poly)->code };
	//model.colors[i][0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_F3*)poly)->code };
    /*model.colors[i][1] = (CVECTOR) { color.r, color.g, color.b, ((POLY_GT3*)poly)->code };
    model.colors[i][2] = (CVECTOR) { color.r, color.g, color.b, ((POLY_GT3*)poly)->code };*/
      mesh->polys[j].colors[0] = (CVECTOR) { tmd_prim.r0, tmd_prim.g0, tmd_prim.b0, ((POLY_F3*)mesh->polys[j].gpu_poly)->code };
      mesh->polys[j].colors[1] = (CVECTOR) { tmd_prim.r0, tmd_prim.g0, tmd_prim.b0, ((POLY_F3*)mesh->polys[j].gpu_poly)->code };
      mesh->polys[j].colors[2] = (CVECTOR) { tmd_prim.r0, tmd_prim.g0, tmd_prim.b0, ((POLY_F3*)mesh->polys[j].gpu_poly)->code };
      mesh->polys[j].colors[3] = (CVECTOR) { tmd_prim.r0, tmd_prim.g0, tmd_prim.b0, ((POLY_F3*)mesh->polys[j].gpu_poly)->code };

    /*setUV3((POLY_GT3*)poly, 
      tmd_prim.u0, tmd_prim.v0,
      tmd_prim.u1, tmd_prim.v1,
      tmd_prim.u2, tmd_prim.v2);

    if (texture != NULL) {
      ((POLY_GT3*)poly)->tpage = texture->tpage;
      ((POLY_GT3*)poly)->clut = texture->clut;
    } else {
      ((POLY_GT3*)poly)->tpage = tmd_prim.tpage;
      ((POLY_GT3*)poly)->clut = tmd_prim.clut;
    }*/
    }
    //printf("Loaded mesh %d with %d polys\n", i, mesh->polys_count);
  }

  //printf("Loaded model with %d mesh(s)\n", model.meshes_count);
  //printf("Loaded model with %d meshes\n", model.meshes_count);

  return model;
}

/*model_Model model_create_cube(int size, CVECTOR color, video_Texture *texture) {
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
}*/

/*model_Model model_create_plane(int size, CVECTOR color, video_Texture *texture) {
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
}*/

/*model_Anim model_load_mime(cd_File file, model_Model model) {
  model_Anim anim;
  long c;

  int i;
  long *ptr = (long*)file.buffer;

  int count = *ptr++;
  long obj_num = *ptr++;
  long offset = *ptr++;
  anim.count = *ptr++;
  anim.diffs = ptr;
  ptr += anim.count * 2;

  printf("count: %d, num: %d, offset: %d, total: %d, addr: 0x%x\n", count, obj_num, offset, anim.count, anim.diffs);*/
  
  /*obj_num = *ptr++;
  offset = *ptr++;
  anim.count = *ptr++;
  anim.diffs = ptr;
  ptr += anim.count * 2;

  printf("count: %d, num: %d, offset: %d, total: %d, addr: 0x%x\n", count, obj_num, offset, anim.count, c);

  obj_num = *ptr++;
  offset = *ptr++;
  anim.count = *ptr++;
  anim.diffs = ptr;
  ptr += anim.count * 2;

  printf("count: %d, num: %d, offset: %d, total: %d, addr: 0x%x\n", count, obj_num, offset, anim.count, c);*/

  /*anim.original_vertices = (SVECTOR*)malloc3(sizeof(SVECTOR) * anim.count);
  //for (i=0; i<anim.count; i++) {
    //anim.original_vertices[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 3);
    //copyVector(&anim.original_vertices[i][0], &model.vertices[i][0]);
    //copyVector(&anim.original_vertices[i][1], &model.vertices[i][1]);
    //copyVector(&anim.original_vertices[i][2], &model.vertices[i][2]);
    memcpy(anim.original_vertices, model.raw_vertices, sizeof(SVECTOR) * anim.count);
  //}

  //anim.vertices = (SVECTOR*)malloc3(sizeof(SVECTOR) * anim.count);
  //for (i=0; i<anim.count; i++) 
  //  copyVector(&anim.vertices[i], &anim.original_vertices[i/3][i%3]);

  return anim;
}*/
