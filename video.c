#include "video.h"
#include "cd.h"

#define OT_SIZE 4096
#define PRIMS_SIZE 65536

DISPENV disp[2];
DRAWENV draw[2];

u_long ot[2][OT_SIZE];
char prims_buffer[2][PRIMS_SIZE];
char *next_prim;

BOOL currentBuffer = 0;
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

  next_prim = prims_buffer[currentBuffer];
  PutDrawEnv(&draw[currentBuffer]);
  PutDispEnv(&disp[currentBuffer]);
  ClearOTag(ot[currentBuffer], OT_SIZE);
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
  //texture.tpage = LoadTPage(tim.paddr, tim.mode, 0, tim.prect->x, tim.prect->y, tim.prect->w, tim.prect->h);
  texture.tpage = getTPage(tim.mode, 2, tim.prect->x, tim.prect->y);
  //DrawSync(0);

  if (tim.caddr) {
    texture.clut = LoadClut(tim.caddr, tim.crect->x, tim.crect->y);
    //DrawSync(0);
  }

  texture.prect = *tim.prect;
  texture.crect = *tim.crect;
  texture.caddr = tim.caddr;
  texture.mode = tim.mode;

  printf("Loaded texture, x: %d y: %d w: %d h: %d mode: %d\n", tim.prect->x, tim.prect->y, tim.prect->w, tim.prect->h, tim.mode);

  return texture;
}

void video_draw() {

  DrawSync(0);
  DrawOTag(ot[currentBuffer]); 
  VSync(0);

  //PutDispEnv(&disp[currentBuffer]);
  //PutDrawEnv(&draw[currentBuffer]);

  //DrawOTag(ot[currentBuffer]); 
  //DrawSync(0);

  currentBuffer = !currentBuffer;
}

void video_draw_sprite(video_Texture texture, int x, int y) {
  SPRT *sprite_prim;
  DR_TPAGE *tpage_prim;
  short u, v;

  sprite_prim = (SPRT*)next_prim;
  next_prim += sizeof(SPRT);
  SetSprt(sprite_prim);
  setXY0(sprite_prim, x, y);
  u = (texture.prect.x % 64) << (2 - (texture.mode & 3));
  v = texture.prect.y & 0xff;
  setUV0(sprite_prim, u, v);
  setWH(sprite_prim, texture.prect.w, texture.prect.h);
  setShadeTex(sprite_prim, 1);
  if (texture.caddr)
    sprite_prim->clut = texture.clut;
  AddPrim(ot[currentBuffer], sprite_prim);

  tpage_prim = (DR_TPAGE*)next_prim;
  next_prim += sizeof(DR_TPAGE);
  SetDrawTPage(tpage_prim, 0, 0, texture.tpage);
  AddPrim(ot[currentBuffer], tpage_prim);
}

void video_draw_poly4(POLY_F4 *poly, SVECTOR vertices[4], CVECTOR color, SVECTOR normal) {
  long outerProduct, otz;
  long dummy1, dummy2;

  outerProduct = RotAverageNclip4(&vertices[0], &vertices[1], &vertices[2], &vertices[3],
    &poly->x0, &poly->x1, &poly->x2, &poly->x3,
    &dummy1, &otz, &dummy2);
  if (outerProduct > 0) {
    NormalColorCol(&normal, &color, &poly->r0);
    AddPrim(ot[currentBuffer] + OT_SIZE - otz, poly);
  }
}

/*void video_draw_poly3(POLY_GT3 *poly, SVECTOR vertices[3], CVECTOR colors[3], SVECTOR normals[3]) {
  POLY_GT3 *poly;
  long outerProduct, otz;
  long dummy1, dummy2;

  poly = (POLY_GT3*)next_prim;
  next_prim += sizeof(POLY_GT3);
  SetPolyGT3(poly);

  outerProduct = RotAverageNclip3(&vertices[0], &vertices[1], &vertices[2],
    &poly->x0, &poly->x1, &poly->x2,
    &dummy1, &otz, &dummy2);
  if (outerProduct > 0) {
    NormalColorCol(&normals[0], &colors[0], &poly->r0);
    NormalColorCol(&normals[1], &colors[1], &poly->r1);
    NormalColorCol(&normals[2], &colors[2], &poly->r2);
    AddPrim(ot[currentBuffer] + OT_SIZE - otz, poly);
  }
}*/

void video_draw_model(model_Model model) {
  POLY_GT3 *poly;
  int i;
  long outerProduct, otz;
  long tmp1, tmp2;

  for (i=0; i<model.polys_count; i++) {
    poly = (POLY_GT3*)next_prim;
    next_prim += sizeof(POLY_GT3);
    SetPolyGT3(poly);

    // If model has textures
    if (model.tpages != NULL && model.cluts != NULL) {
      setUV3(poly, model.uvs[i][0], model.uvs[i][1], model.uvs[i][2], model.uvs[i][3], model.uvs[i][4], model.uvs[i][5]);
      poly->tpage = model.tpages[i];
      poly->clut = model.cluts[i];
    }

    outerProduct = RotAverageNclip3(&model.vertices[i][0], &model.vertices[i][1], &model.vertices[i][2],
      &poly->x0, &poly->x1, &poly->x2,
      &tmp1, &otz, &tmp2);
    if (outerProduct > 0) {
      model.colors[i][0].cd = poly->code;
      model.colors[i][1].cd = poly->code;
      model.colors[i][2].cd = poly->code;
      NormalColorCol(&model.normals[i][0], &model.colors[i][0], &poly->r0);
      NormalColorCol(&model.normals[i][1], &model.colors[i][1], &poly->r1);
      NormalColorCol(&model.normals[i][2], &model.colors[i][2], &poly->r2);
      AddPrim(ot[currentBuffer] + OT_SIZE - otz, poly);
    }
  }
}

