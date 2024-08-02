#include "sprite.h"
#include "video.h"

sprite_Sprite sprite_load_sprite(video_Image image) {
  sprite_Sprite sprite = { NULL, NULL };

  sprite.sprite = (SPRT*)malloc3(sizeof(SPRT));
  setSprt(sprite.sprite);
  setWH(sprite.sprite, image.prect.w, image.prect.h);
  setUV0(sprite.sprite, (image.prect.x % 64) << (2 - (image.mode & 0x3)), (image.prect.y & 0xff));
  setShadeTex(sprite.sprite, 1);
  sprite.sprite->clut = image.clut;

  sprite.tpage_change = (DR_TPAGE*)malloc3(sizeof(DR_TPAGE));
  SetDrawTPage(sprite.tpage_change, 0, 0, image.tpage);

  return sprite;
}
