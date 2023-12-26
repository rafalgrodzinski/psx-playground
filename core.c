#include "core.h"
#include "video.h"
#include "audio.h"
#include "pad.h"
#include "cd.h"

void core_init() {
  // Allocate 1 MiB of heap, leave 64 KiB for stack
  EnterCriticalSection();
  InitHeap3((void*)0x800f0000, 0x00100000);
  ExitCriticalSection();
}

void core_init_all(int mode) {
  core_init();
  video_init(mode);
  audio_init();
  pad_init();
  cd_init();

  video_set_light_color(ONE, ONE, ONE);
}
