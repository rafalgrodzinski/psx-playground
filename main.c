#include <sys/types.h>
#include <stdio.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#include "types.h"
#include "video.h"
#include "audio.h"
#include "pad.h"
#include "cd.h"
#include "sprite.h"
#include "model.h"

VECTOR vec = {0, 0, 1024};
SVECTOR ang = {0, 0, 0};

VECTOR vec2 = {0, 0, 1024};
SVECTOR ang2 = {0, 0, 0};

cd_File file;
model_Model model, model2;
video_Texture texture;
sprite_Sprite sprite, sprite2;


int main() {
  int i;

  InitHeap3((void*)0x801f0000, 0x00100000);

  video_init(VIDEO_HI_RES);
  audio_init();
  pad_init();
  cd_init();

  video_set_light_color(ONE, ONE, ONE);

  file = cd_load_file("\\EARTH8.TIM;1");
  printf("EARTH8.TIM start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  texture = video_load_texture(file);
  cd_free_file(file);
  sprite = sprite_load_sprite(texture);

  file = cd_load_file("\\EARTH.TMD;1");
  printf("EARTH.TMD start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  model2 = model_load_tmd(file, TRUE, &texture);
  cd_free_file(file);

  /*file = cd_load_file("\\DINO.TMD;1");
  model2 = model_load_tmd(file, TRUE, &texture);
  cd_free_file(file);*/

  //file = cd_load_file("\\GIULI_TR.TIM;1");
  //printf("GIULI_TR.TIM start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  //video_load_texture(file);
  //cd_free_file(file);

  //file = cd_load_file("\\GIULI_WN.TIM;1");
  //printf("GIULI_WN.TIM start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  //video_load_texture(file);
  //cd_free_file(file);

  //file = cd_load_file("\\GIULI_BD.TIM;1");
  //printf("GIULI_BD.TIM start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  //video_load_texture(file);
  //cd_free_file(file);

  //file = cd_load_file("\\GIULI_TL.TIM;1");
  //printf("GIULI_TL.TIM start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  //video_load_texture(file);
  //cd_free_file(file);

  //file = cd_load_file("\\GIULI_PL.TIM;1");
  //printf("GIULI_PL.TIM start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  //video_load_texture(file);
  //cd_free_file(file);

  //file = cd_load_file("\\GIULI_FR.TIM;1");
  //printf("GIULI_FR.TIM start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  //video_load_texture(file);
  //cd_free_file(file);

  //file = cd_load_file("\\GIULIETA.TMD;1");
  //printf("GIULIETA.TMD start: 0x%08x, size: 0x%08x\n", file.buffer, file.size);
  //model = model_load_tmd(file, TRUE, NULL);
  //cd_free_file(file);

  //file = cd_load_file("\\FROG.TIM;1");
  //texture = video_load_texture(file);
  //cd_free_file(file);
  //sprite2 = sprite_load_sprite(texture);

DrawSync(0);
  while(1) {
    if (pad_is_pressed(PAD_1, PAD_UP))
      vec.vy -= 32;

    if (pad_is_pressed(PAD_1, PAD_DOWN))
      vec.vy += 32;
    
    if (pad_is_pressed(PAD_1, PAD_LEFT))
      vec.vx -= 32;

    if (pad_is_pressed(PAD_1, PAD_RIGHT))
      vec.vx += 32;

    if (pad_is_pressed(PAD_1, PAD_L1))
      vec.vz += 32;

    if (pad_is_pressed(PAD_1, PAD_L2))
      vec.vz -= 32;

    if (pad_is_pressed(PAD_1, PAD_CIRCLE))
      ang.vy -= 16;

    if (pad_is_pressed(PAD_1, PAD_SQUARE))
      ang.vy += 16;

    if (pad_is_pressed(PAD_1, PAD_TRIANGLE))
      ang.vx -= 16;

    if (pad_is_pressed(PAD_1, PAD_CROSS))
      ang.vx += 16;

    if (pad_is_pressed(PAD_1, PAD_R1))
      ang.vz -= 16;

    if (pad_is_pressed(PAD_1, PAD_R2))
      ang.vz += 16;

    video_set_camera(vec, ang);
    video_init_frame();
    
    //for (i=0; i<model.count; i++)
    //  video_draw_poly3(&model.polys[i], model.vertices[i], model.colors[i], model.normals[i]);

    for (i=0; i<model2.count; i++)
      video_draw_poly3(&model2.polys[i], model2.vertices[i], model2.colors[i], model2.normals[i]);

    //video_draw_sprite(sprite, 0, 0);
    //video_draw_sprite(sprite2, 150, 150);

    video_draw();
  }
}
