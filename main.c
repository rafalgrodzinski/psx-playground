#include <sys/types.h>
#include <stdio.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#include "core.h"
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
model_Model model0, model1, model2, model3, model4;
video_Texture texture0, texture, texture2;
sprite_Sprite sprite1, sprite2;


int main() {
  int i;

  core_init_all(VIDEO_HI_RES);

  file = cd_load_file("\\EARTH8.TIM;1");
  texture = video_load_texture(file);
  cd_free_file(file);
  sprite1 = sprite_load_sprite(texture);

  //file = cd_load_file("\\FROG.TIM;1");
  //texture2 = video_load_texture(file);
  //cd_free_file(file);
  //sprite2 = sprite_load_sprite(texture);

  //file = cd_load_file("\\CRATE.TIM;1");
  //texture0 = video_load_texture(file);
  //cd_free_file(file);
  //sprite2 = sprite_load_sprite(texture0);

  //model0 = model_create_plane(500, (CVECTOR) { 128, 128, 128, 0}, &texture0);

  file = cd_load_file("\\EARTH.TMD;1");
  model1 = model_load_tmd(file, (CVECTOR) { 128, 128, 128, 0 } , &texture);
  cd_free_file(file);

  /*file = cd_load_file("\\DINO.TMD;1");
  model2 = model_load_tmd(file, TRUE, &texture);
  cd_free_file(file);
  */

  file = cd_load_file("\\GIULI_TR.TIM;1");
  video_load_texture(file);
  cd_free_file(file);

  file = cd_load_file("\\GIULI_WN.TIM;1");
  video_load_texture(file);
  cd_free_file(file);

  file = cd_load_file("\\GIULI_BD.TIM;1");
  video_load_texture(file);
  cd_free_file(file);

  file = cd_load_file("\\GIULI_TL.TIM;1");
  video_load_texture(file);
  cd_free_file(file);

  file = cd_load_file("\\GIULI_PL.TIM;1");
  video_load_texture(file);
  cd_free_file(file);

  file = cd_load_file("\\GIULI_FR.TIM;1");
  video_load_texture(file);
  cd_free_file(file);

  file = cd_load_file("\\GIULIETA.TMD;1");
  model3 = model_load_tmd(file, (CVECTOR) { 128, 128, 128 } , &texture);
  cd_free_file(file);

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

    //video_draw_model(model4);
    //video_draw_model(model0);
    video_draw_model(model3);
    video_draw_model(model1);

    //video_draw_model(model4);

    
    //for (i=0; i<model1.count; i++)
    //  video_draw_poly3(&model1.polys[i], model1.vertices[i], model1.colors[i], model1.normals[i]);

    //for (i=0; i<model2.count; i++)
    //  video_draw_poly3(&model2.polys[i], model2.vertices[i], model2.colors[i], model2.normals[i]);

    //for (i=0; i<model3.polys_count; i++)
    //  video_draw_poly3(&model3.polys[i], model3.vertices[i], model3.colors[i], model3.normals[i]);

    //for (i=0; i<model4.polys_count; i++)
    //  video_draw_poly4(&model4.f4_polys[i], model4.vertices[i], model4.colors[i][0], model4.normals[i][0]);
    //video_draw_model(model3);

    //video_draw_sprite(sprite1, 0, 0);
    //video_draw_sprite(sprite2, 150, 150);
    //
    //video_draw_sprite(sprite2, 10, 10);

    video_draw();
  }
}
