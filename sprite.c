#include "sprite.h"
#include "video.h"

sprite_Sprite sprite_load_sprite(video_Texture texture) {
  sprite_Sprite sprite = { NULL, NULL };

  sprite.sprite = (SPRT*)malloc3(sizeof(SPRT));
  setSprt(sprite.sprite);
  setWH(sprite.sprite, texture.prect.w, texture.prect.h);
  setUV0(sprite.sprite, (texture.prect.x % 64) << (2 - (texture.mode & 0x3)), (texture.prect.y & 0xff));
  setShadeTex(sprite.sprite, 1);

  if (texture.caddr)
    sprite.sprite->clut = texture.clut;

  sprite.tpage_change = (DR_TPAGE*)malloc3(sizeof(DR_TPAGE));
  SetDrawTPage(sprite.tpage_change, 0, 0, texture.tpage);

  return sprite;
}
