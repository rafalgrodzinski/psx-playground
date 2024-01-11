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


model_Object test_object;
model_Anim test_anim;

int main() {
  int i;

  core_init_all(VIDEO_HI_RES, (CVECTOR) { 20, 20, 20 }, FPS_LIMIT_60, TRUE);
  load_data();

  while(1) {
    if (pad_is_pressed(PAD_1, PAD_UP))
      camera_offset.vy -= 32;

    if (pad_is_pressed(PAD_1, PAD_DOWN))
      camera_offset.vy += 32;
    
    if (pad_is_pressed(PAD_1, PAD_LEFT))
      camera_offset.vx -= 32;

    if (pad_is_pressed(PAD_1, PAD_RIGHT))
      camera_offset.vx += 32;

    if (pad_is_pressed(PAD_1, PAD_L1))
      camera_offset.vz += 32;

    if (pad_is_pressed(PAD_1, PAD_L2))
      camera_offset.vz -= 32;

    if (pad_is_pressed(PAD_1, PAD_CIRCLE))
      test_object.angle.vy -= 16;

    if (pad_is_pressed(PAD_1, PAD_SQUARE))
      test_object.angle.vy += 16;

    if (pad_is_pressed(PAD_1, PAD_TRIANGLE))
      test_object.angle.vx -= 16;

    if (pad_is_pressed(PAD_1, PAD_CROSS))
      test_object.angle.vx += 16;

    if (pad_is_pressed(PAD_1, PAD_R1)) {
      test_object.scale.vx -= 16;
      test_object.scale.vy -= 16;
      test_object.scale.vz -= 16;
    }

    if (pad_is_pressed(PAD_1, PAD_R2)) {
      test_object.scale.vx += 16;
      test_object.scale.vy += 16;
      test_object.scale.vz += 16;
    }

    /*if (pad_is_pressed(PAD_1, PAD_L1))
      car_trans.vx += 8;

    if (pad_is_pressed(PAD_1, PAD_L2))
      car_trans.vx -= 8;*/

    /*if (pad_is_pressed(PAD_1, PAD_R1))
      car_object.offset.vz += 8;

    if (pad_is_pressed(PAD_1, PAD_R2))
      car_object.offset.vz -= 8;*/

    /*if (pad_is_pressed(PAD_1, PAD_START))
      car_object.angle.vy += 8;

    if (pad_is_pressed(PAD_1, PAD_SELECT))
      car_object.angle.vy -= 8;*/

    video_set_camera(camera_offset, camera_angle);
    video_init_frame();

    //video_draw_object(car_object);
    //video_draw_model(floor_model);
    video_draw_object(test_object);

    video_draw();
  }
}

static load_data() {
  cd_File file;
  video_Texture texture;
  void *vertex_buff = malloc3(sizeof(char) * 0x10000);
  void *normal_buff = malloc3(sizeof(char) * 0x10000);

  file = cd_load_file("\\MD0.TMD;1");
  test_object.model = model_load_tmd(file, (CVECTOR) { 128, 128, 128 }, NULL);
  test_object.angle = (SVECTOR) { 0, 0, 0 };
  test_object.offset = (VECTOR) { 0, 0, 0 };
  test_object.scale = (VECTOR) { ONE, ONE, ONE };

  file = cd_load_file("\\MD0.VDF;1");
  test_anim = model_load_mime(file);

  //init_mime_data(0,  file1.buffer, file2.buffer, file3.buffer, vertex_buff, normal_buff);
}
