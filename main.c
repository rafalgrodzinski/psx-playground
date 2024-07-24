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
long p = 0;

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

    if (pad_is_pressed(PAD_1, PAD_START)) {
      p += 32;
    }

    if (pad_is_pressed(PAD_1, PAD_SELECT)) {
      p -= 32;
    }
	
	//printf("vx: %d, vy: %d, vz: %d, sx: %d, sy: %d, sz: %d, p: %d\n", camera_offset.vx, camera_offset.vy, camera_offset.vz, test_object.scale.vx, test_object.scale.vy, test_object.scale.vz, p);

    //video_animate_model(test_object.model, test_anim, p);

    video_set_camera(camera_offset, camera_angle);
    video_init_frame();
    video_draw_object(test_object);
    video_draw();
  }
}

static load_data() {
  cd_File file;
  video_Texture texture;
  void *vertex_buff = malloc3(sizeof(char) * 0x10000);
  void *normal_buff = malloc3(sizeof(char) * 0x10000);

  //file = cd_load_file("\\DINO.TMD;1");
  //file = cd_load_file("\\MD0.TMD;1");
  //file = cd_load_file("\\GIULIETA.TMD;1");
  //file = cd_load_file("\\SHUTTLE.TMD;1");
  file = cd_load_file("\\TE.TMD;1");
  test_object.model = model_load_tmd(file, (CVECTOR) { 128, 128, 128 }, NULL);
  test_object.angle = (SVECTOR) { 0, 0, 0 };
  //test_object.offset = (VECTOR) { 0, 0, 0 };
  test_object.offset = (VECTOR) { 0, 0, 3500 };
  //test_object.scale = (VECTOR) { ONE, ONE, ONE };
  test_object.scale = (VECTOR) { 2000, 2000, 2000 };

  //file = cd_load_file("\\DINO.VDF;1");
  //file = cd_load_file("\\MD0.VDF;1");
  //test_anim = model_load_mime(file, test_object.model);

  //init_mime_data(0,  file1.buffer, file2.buffer, file3.buffer, vertex_buff, normal_buff);
}
