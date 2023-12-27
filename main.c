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

VECTOR camera_offset = {0, 0, 512};
SVECTOR camera_angle = {0, 0, 0};

model_Model floor_model;
model_Object car_object;

int main() {
  int i;

  core_init_all(VIDEO_HI_RES, (CVECTOR) { 20, 20, 20 }, FPS_LIMIT_60, TRUE);
  load_data();

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

    /*if (pad_is_pressed(PAD_1, PAD_R1))
      ang.vz -= 16;

    if (pad_is_pressed(PAD_1, PAD_R2))
      ang.vz += 16;*/

    /*if (pad_is_pressed(PAD_1, PAD_L1))
      car_trans.vx += 8;

    if (pad_is_pressed(PAD_1, PAD_L2))
      car_trans.vx -= 8;*/

    if (pad_is_pressed(PAD_1, PAD_R1))
      car_object.offset.vz += 8;

    if (pad_is_pressed(PAD_1, PAD_R2))
      car_object.offset.vz -= 8;

    if (pad_is_pressed(PAD_1, PAD_START))
      car_object.angle.vy += 8;

    if (pad_is_pressed(PAD_1, PAD_SELECT))
      car_object.angle.vy -= 8;

    video_set_camera(camera_offset, camera_angle);
    video_init_frame();

    video_draw_object(car_object);
    video_draw_model(floor_model);

    video_draw();
  }
}

static load_data() {
  cd_File file;
  video_Texture texture;

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
  car_object.model = model_load_tmd(file, (CVECTOR) { 128, 128, 128 } , NULL);
  cd_free_file(file);
  car_object.angle = (SVECTOR) { 0, 0, 0 };
  car_object.offset = (VECTOR) { 0, 0, 0 };

  floor_model = model_create_plane(600, (CVECTOR) { 32, 128, 32 }, NULL);
}
