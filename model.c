#include "model.h"
#include "video.h"
#include "cd.h"

model_Model model_load_tmd(cd_File file, video_Image *image) {
  model_Model model;
  TMD_PRIM tmd_prim;
  int i, j;
  u_long *ptr = (u_long*)file.buffer;
  u_char is_address_absolute;
  
  is_address_absolute = *ptr++ & 0x1; // id
  ptr++; // flags
  model.meshes_count = *ptr++;
  model.meshes = malloc3(sizeof(model_Mesh) * model.meshes_count);
  
  printf("Loading model with %d meshes\n", model.meshes_count);
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
        //printf("FT3\n");
        mesh->polys[j].type = model_Poly_Type_FT3;
        mesh->polys[j].gpu_poly = malloc3(sizeof(POLY_FT3));
        SetPolyFT3((POLY_FT3*)mesh->polys[j].gpu_poly);
      // FT4
      } else if (is_quad && is_textured && !is_gouard_shaded) {
        //printf("FT4\n");
        mesh->polys[j].type = model_Poly_Type_FT4;
        mesh->polys[j].gpu_poly = malloc3(sizeof(POLY_FT4));
        SetPolyFT4((POLY_FT4*)mesh->polys[j].gpu_poly);
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
        //printf("GT3\n");
        mesh->polys[j].type = model_Poly_Type_GT3;
        mesh->polys[j].gpu_poly = malloc3(sizeof(POLY_GT3));
        SetPolyGT3((POLY_GT3*)mesh->polys[j].gpu_poly);
      // GT4
      } else if (is_quad && is_textured && is_gouard_shaded) {
        //printf("GT4\n");
        mesh->polys[j].type = model_Poly_Type_GT4;
        mesh->polys[j].gpu_poly = malloc3(sizeof(POLY_GT4));
        SetPolyGT4((POLY_GT4*)mesh->polys[j].gpu_poly);
      // Others are unsupported
      } else {
        printf("Unsupported poly type\n");
        continue;
      }

      mesh->polys[j].vertice_offsets[0] = tmd_prim.vert0;
      mesh->polys[j].vertice_offsets[1] = tmd_prim.vert1;
      mesh->polys[j].vertice_offsets[2] = tmd_prim.vert2;
      mesh->polys[j].vertice_offsets[3] = tmd_prim.vert3;

      copyVector(&mesh->polys[j].normals[0], &tmd_prim.n0);
      copyVector(&mesh->polys[j].normals[1], &tmd_prim.n1);
      copyVector(&mesh->polys[j].normals[2], &tmd_prim.n2);
      copyVector(&mesh->polys[j].normals[3], &tmd_prim.n3);
    
      if (is_textured) {
        mesh->polys[j].colors[0] = (CVECTOR) { 31, 31, 31, ((POLY_FT4*)mesh->polys[j].gpu_poly)->code };
        mesh->polys[j].colors[1] = (CVECTOR) { 31, 31, 31, ((POLY_FT4*)mesh->polys[j].gpu_poly)->code };
        mesh->polys[j].colors[2] = (CVECTOR) { 31, 31, 31, ((POLY_FT4*)mesh->polys[j].gpu_poly)->code };
        mesh->polys[j].colors[3] = (CVECTOR) { 31, 31, 31, ((POLY_FT4*)mesh->polys[j].gpu_poly)->code };
      } else {
        mesh->polys[j].colors[0] = (CVECTOR) { tmd_prim.r0, tmd_prim.g0, tmd_prim.b0, ((POLY_F4*)mesh->polys[j].gpu_poly)->code };
        mesh->polys[j].colors[1] = (CVECTOR) { tmd_prim.r1, tmd_prim.g1, tmd_prim.b1, ((POLY_F4*)mesh->polys[j].gpu_poly)->code };
        mesh->polys[j].colors[2] = (CVECTOR) { tmd_prim.r2, tmd_prim.g2, tmd_prim.b2, ((POLY_F4*)mesh->polys[j].gpu_poly)->code };
        mesh->polys[j].colors[3] = (CVECTOR) { tmd_prim.r3, tmd_prim.g3, tmd_prim.b3, ((POLY_F4*)mesh->polys[j].gpu_poly)->code };
      }

      if (is_textured && !is_gouard_shaded) {
        setUV4((POLY_FT4*)mesh->polys[j].gpu_poly, 
          tmd_prim.u0, tmd_prim.v0,
          tmd_prim.u1, tmd_prim.v1,
          tmd_prim.u2, tmd_prim.v2,
          tmd_prim.u3, tmd_prim.v3);
          
          if (image != NULL) {
            ((POLY_FT4*)mesh->polys[j].gpu_poly)->tpage =  image->tpage;
            ((POLY_FT4*)mesh->polys[j].gpu_poly)->clut = image->clut;
          } else {
            ((POLY_FT4*)mesh->polys[j].gpu_poly)->tpage = tmd_prim.tpage;
            ((POLY_FT4*)mesh->polys[j].gpu_poly)->clut = tmd_prim.clut;
          }

        SetShadeTex((POLY_FT4*)mesh->polys[j].gpu_poly, 1);
      } else if (is_textured && is_gouard_shaded) {
        setUV4((POLY_GT4*)mesh->polys[j].gpu_poly, 
          tmd_prim.u0, tmd_prim.v0,
          tmd_prim.u1, tmd_prim.v1,
          tmd_prim.u2, tmd_prim.v2,
          tmd_prim.u3, tmd_prim.v3);
          
          if (image != NULL) {
            ((POLY_GT4*)mesh->polys[j].gpu_poly)->tpage =  image->tpage;
            ((POLY_GT4*)mesh->polys[j].gpu_poly)->clut = image->clut;
          } else {
            ((POLY_GT4*)mesh->polys[j].gpu_poly)->tpage = tmd_prim.tpage;
            ((POLY_GT4*)mesh->polys[j].gpu_poly)->clut = tmd_prim.clut;
          }

         SetShadeTex((POLY_GT4*)mesh->polys[j].gpu_poly, 1);
      }
    }
  }
  return model;
}

model_Model model_create_cube(int size, CVECTOR color, video_Image *image) {
  model_Model model;
  int i;
  
  SVECTOR n0 = { ONE, 0, 0, 0};
  SVECTOR n1 = { -ONE, 0, 0, 0};
  SVECTOR n2 = { 0, ONE, 0, 0};
  SVECTOR n3 = { 0, -ONE, 0, 0};
  SVECTOR n4 = { 0, 0, ONE, 0};
  SVECTOR n5 = { 0, 0, -ONE, 0};
  
  SVECTOR *normals[6] = { &n5, &n0, &n4, &n1, &n3, &n2 };

  int vertice_offsets[6][4] = {
    { 0, 1, 3, 2 },
    { 1, 5, 2, 6 },
    { 5, 4, 6, 7 },
    { 4, 0, 7, 3 },
    { 4, 5, 0, 1 },
    { 6, 7, 2, 3 }
  };
  
  model.meshes_count = 1;
  model.meshes = malloc3(sizeof(model_Mesh));

  model.meshes[0].polys_count = 6;
  model.meshes[0].polys = malloc(sizeof(model_Poly) * 6);

  model.meshes[0].vertices = (SVECTOR*)malloc3(sizeof(SVECTOR) * 8);
  model.meshes[0].vertices[0] = (SVECTOR) { -size/2, -size/2, -size/2, 0 };
  model.meshes[0].vertices[1] = (SVECTOR) { size/2, -size/2, -size/2, 0 };
  model.meshes[0].vertices[2] = (SVECTOR) { size/2, size/2, -size/2, 0 };
  model.meshes[0].vertices[3] = (SVECTOR) { -size/2, size/2, -size/2, 0 };

  model.meshes[0].vertices[4] = (SVECTOR) { -size/2, -size/2, size/2, 0 };
  model.meshes[0].vertices[5] = (SVECTOR) { size/2, -size/2, size/2, 0 };
  model.meshes[0].vertices[6] = (SVECTOR) { size/2, size/2, size/2, 0 };
  model.meshes[0].vertices[7] = (SVECTOR) { -size/2, size/2, size/2, 0 };

  for (i=0; i<model.meshes[0].polys_count; i++) {
    model.meshes[0].polys[i].vertice_offsets[0] = vertice_offsets[i][0];
    model.meshes[0].polys[i].vertice_offsets[1] = vertice_offsets[i][1];
    model.meshes[0].polys[i].vertice_offsets[2] = vertice_offsets[i][2];
    model.meshes[0].polys[i].vertice_offsets[3] = vertice_offsets[i][3];

    copyVector(model.meshes[0].polys[i].normals, normals[i]);

    if (image != NULL) {
      model.meshes[0].polys[i].type = model_Poly_Type_FT4;
      model.meshes[0].polys[i].gpu_poly = malloc3(sizeof(POLY_FT4));
      SetPolyFT4((POLY_FT4*)model.meshes[0].polys[i].gpu_poly);
      setUVWH((POLY_FT4*)model.meshes[0].polys[i].gpu_poly, 0, 0, image->prect.w, image->prect.h);
      ((POLY_FT4*)model.meshes[0].polys[i].gpu_poly)->tpage = image->tpage;
      ((POLY_FT4*)model.meshes[0].polys[i].gpu_poly)->clut = image->clut;
      model.meshes[0].polys[i].colors[0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_F4*)model.meshes[0].polys[i].gpu_poly)->code };
      
      SetShadeTex((POLY_FT4*)model.meshes[0].polys[i].gpu_poly, 1);
    } else {
      model.meshes[0].polys[i].type = model_Poly_Type_F4;
      model.meshes[0].polys[i].gpu_poly = malloc3(sizeof(POLY_F4));
      SetPolyF4((POLY_F4*)model.meshes[0].polys[i].gpu_poly);
      model.meshes[0].polys[i].colors[0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_F4*)model.meshes[0].polys[i].gpu_poly)->code };
      
      SetShadeTex((POLY_F4*)model.meshes[0].polys[i].gpu_poly, 1);
    }
  }

  printf("Created cube model of size %d\n", size);

  return model;
}

model_Model model_create_plane(int size, CVECTOR color, video_Image *image) {
  model_Model model;

  model.meshes_count = 1;
  model.meshes = malloc3(sizeof(model_Mesh));
  
  model.meshes[0].polys_count = 1;
  model.meshes[0].polys = malloc3(sizeof(model_Poly));

  if (image != NULL) {
    model.meshes[0].polys[0].type = model_Poly_Type_FT4;
    model.meshes[0].polys[0].gpu_poly = malloc3(sizeof(POLY_FT4));
    SetPolyFT4((POLY_FT4*)model.meshes[0].polys[0].gpu_poly);
  } else {
    model.meshes[0].polys[0].type = model_Poly_Type_F4;
    model.meshes[0].polys[0].gpu_poly = malloc3(sizeof(POLY_F4));
    SetPolyF4((POLY_F4*)model.meshes[0].polys[0].gpu_poly);
  }

  model.meshes[0].vertices = (SVECTOR*)malloc3(sizeof(SVECTOR) * 4);
  model.meshes[0].vertices[0] =  (SVECTOR) { -size/2, 0, size/2, 0};
  model.meshes[0].polys[0].vertice_offsets[0] = 0;
  model.meshes[0].vertices[1] =  (SVECTOR) { size/2, 0, size/2, 0};
  model.meshes[0].polys[0].vertice_offsets[1] = 1;
  model.meshes[0].vertices[2] =  (SVECTOR) { -size/2, 0, -size/2, 0};
  model.meshes[0].polys[0].vertice_offsets[2] = 2;
  model.meshes[0].vertices[3] =  (SVECTOR) { size/2, 0, -size/2, 0};
  model.meshes[0].polys[0].vertice_offsets[3] = 3;
  model.meshes[0].polys[0].normals[0] = (SVECTOR) { 0, -ONE, 0, 0};

  if (image != NULL) {
    model.meshes[0].polys[0].colors[0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_FT4*)model.meshes[0].polys[0].gpu_poly)->code };
    setUVWH((POLY_FT4*)model.meshes[0].polys[0].gpu_poly, 0, 0, image->prect.w, image->prect.h);
    ((POLY_FT4*)model.meshes[0].polys[0].gpu_poly)->tpage = image->tpage;
    ((POLY_FT4*)model.meshes[0].polys[0].gpu_poly)->clut = image->clut;
  } else {
    model.meshes[0].polys[0].colors[0] = (CVECTOR) { color.r, color.g, color.b, ((POLY_F4*)model.meshes[0].polys[0].gpu_poly)->code };
  }

  printf("Created plane model of size %d\n", size);

  return model;
}

model_Anim model_load_mime(cd_File file, model_Model model) {
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

  printf("count: %d, num: %d, offset: %d, total: %d, addr: 0x%x\n", count, obj_num, offset, anim.count, anim.diffs);

  anim.original_vertices = (SVECTOR*)malloc3(sizeof(SVECTOR) * anim.count);
  //for (i=0; i<anim.count; i++) {
    //anim.original_vertices[i] = (SVECTOR*)malloc3(sizeof(SVECTOR) * 3);
    //copyVector(&anim.original_vertices[i][0], &model.vertices[i][0]);
    //copyVector(&anim.original_vertices[i][1], &model.vertices[i][1]);
    //copyVector(&anim.original_vertices[i][2], &model.vertices[i][2]);
    memcpy(anim.original_vertices, model.meshes[0].vertices, sizeof(SVECTOR) * anim.count);
  //}

  //anim.vertices = (SVECTOR*)malloc3(sizeof(SVECTOR) * anim.count);
  //for (i=0; i<anim.count; i++) 
  //  copyVector(&anim.vertices[i], &anim.original_vertices[i/3][i%3]);

  return anim;
}
