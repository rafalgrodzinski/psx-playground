#include "video.h"
#include "sprite.h"
#include "model.h"
#include "cd.h"

#define OT_SIZE 4096

DISPENV disp[2];
DRAWENV draw[2];

u_long ot[2][OT_SIZE];
BOOL current_buffer;
MATRIX camera_matrix;
MATRIX light_matrix;

BOOL should_show_fps;
int fps, frames_count, vsyncs_count;
int fps_limit;

static void video_vsync_callback() {
  vsyncs_count++;
  if (vsyncs_count > 60) {
    if (frames_count > 0)
      fps = frames_count - 1;
    frames_count = vsyncs_count = 0;
  }
}

void video_init(int mode, CVECTOR background_color, int _fps_limit, BOOL _should_show_fps) {
  short width = 320;
  short height = 240;
  short offset = 240;
  short text_offset = 8;
  BOOL is_interlaced = FALSE;
  should_show_fps = _should_show_fps;
  fps_limit = _fps_limit;

  if (mode == VIDEO_HI_RES) {
    width = 640;
    height = 480;
    offset = 0;
    text_offset = 16;
    is_interlaced = TRUE;
  }

  ResetCallback();
  ResetGraph(0);
  SetVideoMode(MODE_NTSC);

  SetDefDispEnv(&disp[0], 0, 0, width, height);
  SetDefDispEnv(&disp[1], 0, offset, width, height);

  disp[0].isinter = is_interlaced;
  disp[1].isinter = is_interlaced;

  SetDefDrawEnv(&draw[0], 0, offset, width, height);
  SetDefDrawEnv(&draw[1], 0, 0, width, height);

  setRGB0(&draw[0], background_color.r, background_color.g, background_color.b);
  setRGB0(&draw[1], background_color.r, background_color.g, background_color.b);

  draw[0].isbg = 1;
  draw[0].dtd = 0;
  draw[1].isbg = 1;
  draw[1].dtd = 0;

  FntLoad(960, 0);
  SetDumpFnt(FntOpen(0, text_offset, 320, 224, 0, 100));

  InitGeom();
  SetGeomOffset(width / 2, height / 2);
  SetGeomScreen(1024);

  VSyncCallback(video_vsync_callback);

  SetDispMask(1);
}

void video_init_frame() {
  ClearOTag(ot[current_buffer], OT_SIZE);

  if (should_show_fps) {
    FntPrint("FPS: %d", fps);
    FntFlush(-1);
  }
}

void video_draw() {
  frames_count++;
  DrawSync(0);
  VSync(fps_limit);
  PutDispEnv(&disp[current_buffer]);
  PutDrawEnv(&draw[current_buffer]);
  DrawOTag(ot[current_buffer]); 
  current_buffer = !current_buffer;
}

void video_set_camera(VECTOR offset, SVECTOR angle) {
    RotMatrix(&angle, &camera_matrix);
    TransMatrix(&camera_matrix, &offset);
}

void video_set_1_light(SVECTOR color, SVECTOR angle, CVECTOR ambient_color) {
    MATRIX color_matrix = {
        color.vx, 0, 0,
        color.vy, 0, 0,
        color.vz, 0, 0
    };

    SetColorMatrix(&color_matrix);

    light_matrix = (MATRIX) {
        angle.vx, angle.vy, angle.vz,
        0, 0, 0,
        0, 0, 0
    };

    SetBackColor(ambient_color.r, ambient_color.g, ambient_color.b);
}

void video_set_2_lights(SVECTOR color0, SVECTOR angle0, SVECTOR color1, SVECTOR angle1, CVECTOR ambient_color) {
    MATRIX color_matrix = {
        color0.vx, color1.vx, 0,
        color0.vy, color1.vy, 0,
        color0.vz, color1.vz, 0
    };

    SetColorMatrix(&color_matrix);

    light_matrix = (MATRIX) {
        angle0.vx, angle0.vy, angle0.vz,
        angle1.vx, angle1.vy, angle1.vz,
        0, 0, 0
    };

    SetBackColor(ambient_color.r, ambient_color.g, ambient_color.b);
}

void video_set_3_lights(SVECTOR color0, SVECTOR angle0, SVECTOR color1, SVECTOR angle1, SVECTOR color2, SVECTOR angle2, CVECTOR ambient_color) {
    MATRIX color_matrix = {
        color0.vx, color1.vx, color2.vx,
        color0.vy, color1.vy, color2.vy,
        color0.vz, color1.vz, color2.vz
    };

    SetColorMatrix(&color_matrix);

    light_matrix = (MATRIX) {
        angle0.vx, angle0.vy, angle0.vz,
        angle1.vx, angle1.vy, angle1.vz,
        angle2.vx, angle2.vy, angle2.vz
    };

    SetBackColor(ambient_color.r, ambient_color.g, ambient_color.b);
}

video_Texture video_load_texture(cd_File file) {
  video_Texture texture;
  TIM_IMAGE tim;
  int i;
  
  // First figure out number of images
  texture.images_count = 0;
  OpenTIM((u_long*)file.buffer);
  while (ReadTIM(&tim) != NULL)
    texture.images_count++;
  
  printf("Loading texture with %d image(s)\n", texture.images_count);
  
  texture.images = (video_Image*)malloc3(sizeof(video_Image) * texture.images_count);

  // Then read the data
  OpenTIM((u_long*)file.buffer);
  for (i=0; i<texture.images_count && ReadTIM(&tim) != NULL; i++) {
    LoadImage(tim.prect, tim.paddr);
    texture.images[i].tpage = getTPage(tim.mode, 0, tim.prect->x, tim.prect->y);

    if (tim.caddr) {
      texture.images[i].clut = LoadClut(tim.caddr, tim.crect->x, tim.crect->y);
    }

    texture.images[i].prect = *tim.prect;
    texture.images[i].mode = tim.mode;
    printf("Loaded image, x: %d y: %d w: %d h: %d mode: %d\n", tim.prect->x, tim.prect->y, tim.prect->w, tim.prect->h, tim.mode);
  }

  return texture;
}

void video_draw_sprite(sprite_Sprite sprite, int x, int y) {
  setXY0(sprite.sprite, x, y);
  AddPrim(ot[current_buffer], sprite.sprite);
  AddPrim(ot[current_buffer], sprite.tpage_change);
}

void video_draw_model(model_Model model) {
  int i, j;
  
  for (i=0; i<model.meshes_count; i++) {
    model_Mesh *mesh = &model.meshes[i];
    
    for (j=0; j<mesh->polys_count; j++) {
      model_Poly *poly = &mesh->polys[j];
      
      // F3
      if (poly->type == model_Poly_Type_F3) {
        POLY_F3 *gpu_poly = (POLY_F3*)poly->gpu_poly;
        SVECTOR vertices[3];
        vertices[0] = mesh->vertices[poly->vertice_offsets[0]];
        vertices[1] = mesh->vertices[poly->vertice_offsets[1]];
        vertices[2] = mesh->vertices[poly->vertice_offsets[2]];

        video_draw_poly_f3(gpu_poly, vertices, poly->colors[0], poly->normals[0]);
      // F4
      } else if (poly->type == model_Poly_Type_F4) {
        POLY_F4 *gpu_poly = (POLY_F4*)poly->gpu_poly;
        SVECTOR vertices[3];
        vertices[0] = mesh->vertices[poly->vertice_offsets[0]];
        vertices[1] = mesh->vertices[poly->vertice_offsets[1]];
        vertices[2] = mesh->vertices[poly->vertice_offsets[2]];
        vertices[3] = mesh->vertices[poly->vertice_offsets[3]];

        video_draw_poly_f4(gpu_poly, vertices, poly->colors[0], poly->normals[0]);
      // FT3
      } else if (poly->type == model_Poly_Type_FT3) {
        POLY_FT3 *gpu_poly = (POLY_FT3*)poly->gpu_poly;
        SVECTOR vertices[3];
        vertices[0] = mesh->vertices[poly->vertice_offsets[0]];
        vertices[1] = mesh->vertices[poly->vertice_offsets[1]];
        vertices[2] = mesh->vertices[poly->vertice_offsets[2]];

        video_draw_poly_ft3(gpu_poly, vertices, poly->colors[0], poly->normals[0]);
      // FT4
      } else if (poly->type == model_Poly_Type_FT4) {
        POLY_FT4 *gpu_poly = (POLY_FT4*)poly->gpu_poly;
        SVECTOR vertices[4];
        vertices[0] = mesh->vertices[poly->vertice_offsets[0]];
        vertices[1] = mesh->vertices[poly->vertice_offsets[1]];
        vertices[2] = mesh->vertices[poly->vertice_offsets[2]];
        vertices[3] = mesh->vertices[poly->vertice_offsets[3]];

        video_draw_poly_ft4(gpu_poly, vertices, poly->colors[0], poly->normals[0]);
      // G3
      } else if (poly->type == model_Poly_Type_G3) {
        POLY_G3 *gpu_poly = (POLY_G3*)poly->gpu_poly;
        SVECTOR vertices[3];
        vertices[0] = mesh->vertices[poly->vertice_offsets[0]];
        vertices[1] = mesh->vertices[poly->vertice_offsets[1]];
        vertices[2] = mesh->vertices[poly->vertice_offsets[2]];

        video_draw_poly_g3(gpu_poly, vertices, poly->colors, poly->normals);
      // G4
      } else if (poly->type == model_Poly_Type_G4) {
        POLY_G4 *gpu_poly = (POLY_G4*)poly->gpu_poly;
        SVECTOR vertices[4];
        vertices[0] = mesh->vertices[poly->vertice_offsets[0]];
        vertices[1] = mesh->vertices[poly->vertice_offsets[1]];
        vertices[2] = mesh->vertices[poly->vertice_offsets[2]];
        vertices[3] = mesh->vertices[poly->vertice_offsets[2]];

        video_draw_poly_g4(gpu_poly, vertices, poly->colors, poly->normals);
      // GT3
      } else if (poly->type == model_Poly_Type_GT3) {
        POLY_GT3 *gpu_poly = (POLY_GT3*)poly->gpu_poly;
        SVECTOR vertices[3];
        vertices[0] = mesh->vertices[poly->vertice_offsets[0]];
        vertices[1] = mesh->vertices[poly->vertice_offsets[1]];
        vertices[2] = mesh->vertices[poly->vertice_offsets[2]];

        video_draw_poly_gt3(gpu_poly, vertices, poly->colors, poly->normals);
      // GT4
      } else if (poly->type == model_Poly_Type_GT4) {
        POLY_GT4 *gpu_poly = (POLY_GT4*)poly->gpu_poly;
        SVECTOR vertices[4];
        vertices[0] = mesh->vertices[poly->vertice_offsets[0]];
        vertices[1] = mesh->vertices[poly->vertice_offsets[1]];
        vertices[2] = mesh->vertices[poly->vertice_offsets[2]];
        vertices[3] = mesh->vertices[poly->vertice_offsets[3]];

        video_draw_poly_gt4(gpu_poly, vertices, poly->colors, poly->normals);
      }
    }
  }
}

void video_draw_object(model_Object object) {
    MATRIX object_matrix, out_matrix;

    RotMatrix(&object.angle, &object_matrix);
    TransMatrix(&object_matrix, &object.offset);
    ScaleMatrix(&object_matrix, &object.scale);

    MulMatrix0(&light_matrix, &object_matrix, &out_matrix);
    SetLightMatrix(&out_matrix);

    CompMatrixLV(&camera_matrix, &object_matrix, &out_matrix);

    SetRotMatrix(&out_matrix);
    SetTransMatrix(&out_matrix);
  
    video_draw_model(object.model);
}

void video_draw_poly_f3(POLY_F3 *poly, SVECTOR vertices[3], CVECTOR color, SVECTOR normal) {
	long outerProduct, otz;
	long tmp;
	
	outerProduct = RotAverageNclip3(&vertices[0], &vertices[1], &vertices[2],
		&poly->x0, &poly->x1, &poly->x2,
		&tmp, &otz, &tmp);
	if (outerProduct > 0 && otz > 0 && otz <= OT_SIZE) {
		NormalColorCol(&normal, &color, &poly->r0);
		AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
	}
}

void video_draw_poly_f4(POLY_F4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal) {
    long outerProduct, otz;
    long tmp;

    outerProduct = RotAverageNclip4(&vertices[0], &vertices[1], &vertices[2], &vertices[3],
      &poly->x0, &poly->x1, &poly->x2, &poly->x3,
      &tmp, &otz, &tmp);
    if (outerProduct > 0 && otz > 0 && otz <= OT_SIZE) {
        NormalColorCol(&normal, &color, &poly->r0);
        AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
    }
}

void video_draw_poly_ft3(POLY_FT3 *poly, SVECTOR vertices[3], CVECTOR color, SVECTOR normal) {
    long outerProduct, otz;
    long tmp;

    outerProduct = RotAverageNclip3(&vertices[0], &vertices[1], &vertices[2],
        &poly->x0, &poly->x1, &poly->x2,
        &tmp, &otz, &tmp);
    if (outerProduct > 0 && otz > 0 && otz <= OT_SIZE) {
        NormalColorCol(&normal, &color, &poly->r0);
        AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
    }
}

void video_draw_poly_ft4(POLY_FT4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal) {
    long outerProduct, otz;
    long tmp;
    
    SetShadeTex(poly, 1);
  
    outerProduct = RotAverageNclip4(&vertices[0], &vertices[1], &vertices[2], &vertices[3],
      &poly->x0, &poly->x1, &poly->x2, &poly->x3,
      &tmp, &otz, &tmp);
    if (outerProduct > 0 && otz > 0 && otz <= OT_SIZE) {
        NormalColorCol(&normal, &color, &poly->r0);
        AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
    }
}

void video_draw_poly_g3(POLY_G3 *poly, SVECTOR vertices[3], CVECTOR colors[3], SVECTOR normals[3]) {
    long outerProduct, otz;
    long tmp;
    int off;
  
    outerProduct = RotAverageNclip3(&vertices[0], &vertices[1], &vertices[2],
        &poly->x0, &poly->x1, &poly->x2,
        &tmp, &otz, &tmp);
    if (outerProduct > 0 && otz > 0 && otz <= OT_SIZE) {
        NormalColorCol(&normals[0], &colors[0], &poly->r0);
        NormalColorCol(&normals[1], &colors[1], &poly->r1);
        NormalColorCol(&normals[2], &colors[2], &poly->r2);
        AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
    }
}

void video_draw_poly_g4(POLY_G4 *poly, SVECTOR vertices[4], CVECTOR colors[4], SVECTOR normals[4]) {
    long outerProduct, otz;
    long tmp;
  
    outerProduct = RotAverageNclip4(&vertices[0], &vertices[1], &vertices[2], &vertices[3],
        &poly->x0, &poly->x1, &poly->x2, &poly->x3,
        &tmp, &otz, &tmp);
    if (outerProduct > 0 && otz > 0 && otz <= OT_SIZE) {
        NormalColorCol(&normals[0], &colors[0], &poly->r0);
        NormalColorCol(&normals[1], &colors[1], &poly->r1);
        NormalColorCol(&normals[2], &colors[2], &poly->r2);
        NormalColorCol(&normals[3], &colors[3], &poly->r3);
        AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
    }
}

void video_draw_poly_gt3(POLY_GT3 *poly, SVECTOR vertices[3], CVECTOR colors[3], SVECTOR normals[3]) {
    long outerProduct, otz;
    long tmp;
  
    outerProduct = RotAverageNclip3(&vertices[0], &vertices[1], &vertices[2],
        &poly->x0, &poly->x1, &poly->x2,
        &tmp, &otz, &tmp);
    if (outerProduct > 0 && otz > 0 && otz <= OT_SIZE) {
        NormalColorCol(&normals[0], &colors[0], &poly->r0);
        NormalColorCol(&normals[1], &colors[1], &poly->r1);
        NormalColorCol(&normals[2], &colors[2], &poly->r2);
        AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
    }
}

void video_draw_poly_gt4(POLY_GT4 *poly, SVECTOR vertices[4], CVECTOR colors[4], SVECTOR normals[4]) {
    long outerProduct, otz;
    long tmp;
  
    outerProduct = RotAverageNclip4(&vertices[0], &vertices[1], &vertices[2], &vertices[3],
        &poly->x0, &poly->x1, &poly->x2, &poly->x3,
        &tmp, &otz, &tmp);
    if (outerProduct > 0 && otz > 0 && otz <= OT_SIZE) {
        NormalColorCol(&normals[0], &colors[0], &poly->r0);
        NormalColorCol(&normals[1], &colors[1], &poly->r1);
        NormalColorCol(&normals[2], &colors[2], &poly->r2);
        NormalColorCol(&normals[3], &colors[3], &poly->r3);
        AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
    }
}

void video_animate_model(model_Model model, model_Anim anim, int n) {
    memcpy(model.meshes[0].vertices, anim.original_vertices, sizeof(SVECTOR) * anim.diffs_count);
    gteMIMefunc(model.meshes[0].vertices, anim.diffs, anim.diffs_count, n);
}

