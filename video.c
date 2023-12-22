#include "video.h"
#include "sprite.h"
#include "model.h"
#include "cd.h"

#define OT_SIZE 4096

DISPENV disp[2];
DRAWENV draw[2];

u_long ot[2][OT_SIZE];
BOOL current_buffer;
MATRIX cameraMatrix;

void video_init(int mode) {
  short width = 320;
  short height = 240;
  short offset = 240;
  BOOL is_interlaced = FALSE;

  if (mode == VIDEO_HI_RES) {
    width = 640;
    height = 480;
    offset = 0;
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

  setRGB0(&draw[0], 20, 20, 20);
  setRGB0(&draw[1], 20, 20, 20);

  draw[0].isbg = 1;
  draw[0].dtd = 0;
  draw[1].isbg = 1;
  draw[1].dtd = 0;

  InitGeom();
  SetGeomOffset(width / 2, height / 2);
  SetGeomScreen(1024);

  SetDispMask(1);
}

void video_init_frame() {
  MATRIX lightInputMatrix = {
    ONE, -ONE, -ONE,
    0, 0, 0,
    0, 0, 0
  };
  MATRIX lightMatrix;

  MulMatrix0(&lightInputMatrix, &cameraMatrix, &lightMatrix);
  SetLightMatrix(&lightMatrix);

  SetTransMatrix(&cameraMatrix);
  SetRotMatrix(&cameraMatrix);

  ClearOTag(ot[current_buffer], OT_SIZE);
}

void video_draw() {
  DrawSync(0);
  VSync(0);
  PutDispEnv(&disp[current_buffer]);
  PutDrawEnv(&draw[current_buffer]);
  DrawOTag(ot[current_buffer]); 

  current_buffer = !current_buffer;
}


void video_set_camera(VECTOR offset, SVECTOR angle) {
  RotMatrix(&angle, &cameraMatrix);
  TransMatrix(&cameraMatrix, &offset);
}

void video_set_local(VECTOR offset, SVECTOR angle) {
  RotMatrix(&angle, &cameraMatrix);
  TransMatrix(&cameraMatrix, &offset);
  SetTransMatrix(&cameraMatrix);
}

void video_reset_local() {
  PopMatrix();
}

void video_set_light_color(short r, short g, short b) {
  MATRIX lightMatrix = {
    r, 0, 0,
    g, 0, 0,
    b, 0, 0,
    0, 0, 0
  };

  SetColorMatrix(&lightMatrix);
}

video_Texture video_load_texture(cd_File file) {
  video_Texture texture;
  TIM_IMAGE tim;

  OpenTIM((u_long*)file.buffer);
  ReadTIM(&tim);
  LoadImage(tim.prect, tim.paddr);
  // Transparency rate 1
  texture.tpage = getTPage(tim.mode, 1, tim.prect->x, tim.prect->y);

  if (tim.caddr)
    texture.clut = LoadClut(tim.caddr, tim.crect->x, tim.crect->y);

  texture.prect = *tim.prect;
  texture.mode = tim.mode;

  printf("Loaded texture, x: %d y: %d w: %d h: %d mode: %d\n", tim.prect->x, tim.prect->y, tim.prect->w, tim.prect->h, tim.mode);

  return texture;
}

void video_draw_sprite(sprite_Sprite sprite, int x, int y) {
  setXY0(sprite.sprite, x, y);
  AddPrim(ot[current_buffer], sprite.sprite);
  AddPrim(ot[current_buffer], sprite.tpage_change);
}

void video_draw_model(model_Model model) {
  int i;
  void *poly;

  if (model.poly_type == model_Poly_Type_F4) {
    for (i=0; i<model.polys_count; i++) {
      poly = &((POLY_F4*)model.polys)[i];
      video_draw_poly_f4(poly, model.vertices[i], model.colors[i][0], model.normals[i][0]);
    }
  } else if (model.poly_type == model_Poly_Type_FT4) {
    for (i=0; i<model.polys_count; i++) {
      poly = &((POLY_FT4*)model.polys)[i];
      video_draw_poly_ft4(poly, model.vertices[i], model.colors[i][0], model.normals[i][0]);
    }
  }
}

void video_draw_poly_f4(POLY_F4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal) {
  long outerProduct, otz;
  long tmp;

  outerProduct = RotAverageNclip4(&vertices[0], &vertices[1], &vertices[2], &vertices[3],
    &poly->x0, &poly->x1, &poly->x2, &poly->x3,
    &tmp, &otz, &tmp);
  if (outerProduct > 0) {
    NormalColorCol(&normal, &color, &poly->r0);
    AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
  }
}

void video_draw_poly_ft4(POLY_FT4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal) {
  long outerProduct, otz;
  long tmp;

  outerProduct = RotAverageNclip4(&vertices[0], &vertices[1], &vertices[2], &vertices[3],
    &poly->x0, &poly->x1, &poly->x2, &poly->x3,
    &tmp, &otz, &tmp);
  if (outerProduct > 0) {
    NormalColorCol(&normal, &color, &poly->r0);
    AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
  }
}

void video_draw_poly_gt3(POLY_GT3 *poly, SVECTOR vertices[3], CVECTOR colors[3], SVECTOR normals[3]) {
  long outerProduct, otz;
  long dummy1, dummy2;

  outerProduct = RotAverageNclip3(&vertices[0], &vertices[1], &vertices[2],
    &poly->x0, &poly->x1, &poly->x2,
    &dummy1, &otz, &dummy2);
  if (outerProduct > 0) {
    NormalColorCol(&normals[0], &colors[0], &poly->r0);
    NormalColorCol(&normals[1], &colors[1], &poly->r1);
    NormalColorCol(&normals[2], &colors[2], &poly->r2);
    AddPrim(ot[current_buffer] + OT_SIZE - otz, poly);
  }
}

