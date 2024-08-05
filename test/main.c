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

model_Object object1;
model_Anim anim;
long anim_p = 0;

model_Object object2;

int main() {
  core_init_all(VIDEO_HI_RES, (CVECTOR) { 20, 20, 20 }, FPS_LIMIT_60, TRUE);
  load_data();
  video_set_1_light((SVECTOR) { ONE, ONE, ONE }, (SVECTOR) { ONE, -ONE, -ONE }, (CVECTOR) { 10, 10, 10 });

  while(1) {
    // Camera
    if (!pad_is_pressed(PAD_1, PAD_L1) && !pad_is_pressed(PAD_1, PAD_L2)) {
        if (pad_is_pressed(PAD_1, PAD_UP))
            camera_offset.vy -= 32;

        if (pad_is_pressed(PAD_1, PAD_DOWN))
            camera_offset.vy += 32;

        if (pad_is_pressed(PAD_1, PAD_LEFT))
            camera_offset.vx -= 32;

        if (pad_is_pressed(PAD_1, PAD_RIGHT))
            camera_offset.vx += 32;

        if (pad_is_pressed(PAD_1, PAD_R1))
            camera_offset.vz += 32;

        if (pad_is_pressed(PAD_1, PAD_R2))
            camera_offset.vz -= 32;

        if (pad_is_pressed(PAD_1, PAD_SQUARE))
            camera_angle.vy -= 16;

        if (pad_is_pressed(PAD_1, PAD_CIRCLE))
            camera_angle.vy += 16;

        if (pad_is_pressed(PAD_1, PAD_TRIANGLE))
            camera_angle.vx -= 16;

        if (pad_is_pressed(PAD_1, PAD_CROSS))
            camera_angle.vx += 16;

        if (pad_is_pressed(PAD_1, PAD_SELECT))
            anim_p -= 32;

        if (pad_is_pressed(PAD_1, PAD_START))
            anim_p += 32;
    }

    // Object 1
    if (pad_is_pressed(PAD_1, PAD_L1)) {
        if (pad_is_pressed(PAD_1, PAD_UP))
            object1.offset.vy -= 32;

        if (pad_is_pressed(PAD_1, PAD_DOWN))
            object1.offset.vy += 32;

        if (pad_is_pressed(PAD_1, PAD_LEFT))
            object1.offset.vx -= 32;

        if (pad_is_pressed(PAD_1, PAD_RIGHT))
            object1.offset.vx += 32;

        if (pad_is_pressed(PAD_1, PAD_R1))
            object1.offset.vz += 32;

        if (pad_is_pressed(PAD_1, PAD_R2))
            object1.offset.vz -= 32;

        if (pad_is_pressed(PAD_1, PAD_SQUARE))
            object1.angle.vy += 16;

        if (pad_is_pressed(PAD_1, PAD_CIRCLE))
            object1.angle.vy -= 16;

        if (pad_is_pressed(PAD_1, PAD_TRIANGLE))
            object1.angle.vx -= 16;

        if (pad_is_pressed(PAD_1, PAD_CROSS))
            object1.angle.vx += 16;

        if (pad_is_pressed(PAD_1, PAD_SELECT)) {
            object1.scale.vx -= 16;
            object1.scale.vy -= 16;
            object1.scale.vz -= 16;
        }

        if (pad_is_pressed(PAD_1, PAD_START)) {
            object1.scale.vx += 16;
            object1.scale.vy += 16;
            object1.scale.vz += 16;
        }
    }

    // Object 2
    if (pad_is_pressed(PAD_1, PAD_L2)) {
        if (pad_is_pressed(PAD_1, PAD_UP))
            object2.offset.vy -= 32;

        if (pad_is_pressed(PAD_1, PAD_DOWN))
            object2.offset.vy += 32;

        if (pad_is_pressed(PAD_1, PAD_LEFT))
            object2.offset.vx -= 32;

        if (pad_is_pressed(PAD_1, PAD_RIGHT))
            object2.offset.vx += 32;

        if (pad_is_pressed(PAD_1, PAD_R1))
            object2.offset.vz += 32;

        if (pad_is_pressed(PAD_1, PAD_R2))
            object2.offset.vz -= 32;

        if (pad_is_pressed(PAD_1, PAD_SQUARE))
            object2.angle.vy += 16;

        if (pad_is_pressed(PAD_1, PAD_CIRCLE))
            object2.angle.vy -= 16;

        if (pad_is_pressed(PAD_1, PAD_TRIANGLE))
            object2.angle.vx -= 16;

        if (pad_is_pressed(PAD_1, PAD_CROSS))
            object2.angle.vx += 16;

        if (pad_is_pressed(PAD_1, PAD_SELECT)) {
            object2.scale.vx -= 16;
            object2.scale.vy -= 16;
            object2.scale.vz -= 16;
        }

        if (pad_is_pressed(PAD_1, PAD_START)) {
            object2.scale.vx += 16;
            object2.scale.vy += 16;
            object2.scale.vz += 16;
        }
    }
	
    video_animate_model(object1.model, anim, anim_p);

    video_init_frame();
    video_set_camera(camera_offset, camera_angle);

    video_draw_object(object1);
    video_draw_object(object2);

    video_draw();
  }
}

static load_data() {
  cd_File file;
  video_Texture texture;
  
  // Earth
  //file = cd_load_file("\\EARTH4.TIM;1");
  //file = cd_load_file("\\EARTH8.TIM;1");
  //texture = video_load_texture(file);
  //file = cd_load_file("\\EARTH.TMD;1");
  //test_object.model = model_load_tmd(file, &texture.images[0]);
  
  // Dino
  //file = cd_load_file("\\DINO.TIM;1");
  //video_load_texture(file);
  //file = cd_load_file("\\DINO.TMD;1");
  //object1.model = model_load_tmd(file, NULL);
  
  // Shutttle
  //file = cd_load_file("\\SHUTTLE.TMD;1");
  //test_object.model = model_load_tmd(file, NULL);
 
  // Giulieta
  //file = cd_load_file("\\GIULI_BD.TIM;1");
  //video_load_texture(file);
  //file = cd_load_file("\\GIULI_FR.TIM;1");
  //video_load_texture(file);
  //file = cd_load_file("\\GIULI_PL.TIM;1");
  //video_load_texture(file);
  //file = cd_load_file("\\GIULI_TL.TIM;1");
  //video_load_texture(file);
  //file = cd_load_file("\\GIULI_TR.TIM;1");
  //video_load_texture(file);
  //file = cd_load_file("\\GIULI_WN.TIM;1");
  //video_load_texture(file);
  //file = cd_load_file("\\GIULIETA.TMD;1");
  //object2.model = model_load_tmd(file, NULL);

  // Te
  //file = cd_load_file("\\TE.TMD;1");
  //object1.model = model_load_tmd(file, NULL);
  
  // MD0
  file = cd_load_file("\\MD0.TMD;1");
  object1.model = model_load_tmd(file, NULL);
  file = cd_load_file("\\MD0.VDF;1");
  anim = model_load_mime(file, object1.model);
  
  // Plane
  //file = cd_load_file("\\CRATE.TIM;1");
  //texture = video_load_texture(file);
  //object1.model = model_create_plane(500, (CVECTOR) { 128, 128, 128 }, &texture.images[0]);

  // Box
  file = cd_load_file("\\CRATE.TIM;1");
  texture = video_load_texture(file);
  object2.model = model_create_cube(500, (CVECTOR) { 128, 128, 128 }, &texture.images[0]);

  //test_object.angle = (SVECTOR) { 0, 0, 0 };
  //test_object.offset = (VECTOR) { 0, 0, 0 };
  object1.offset = (VECTOR) { -500, 0, 3000 };
  object1.scale = (VECTOR) { ONE, ONE, ONE };
  //test_object.scale = (VECTOR) { 2000, 2000, 2000 };
  
  object2.offset = (VECTOR) { 500, 0, 3000 };
  object2.scale = (VECTOR) { ONE, ONE, ONE };
}
