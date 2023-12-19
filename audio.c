#include "audio.h"

void audio_init() {
  SpuCommonAttr common;

  SpuInit();
  SpuInitMalloc(SPU_MALLOC_MAX, (char*)(SPU_MALLOC_RECSIZ * (SPU_MALLOC_MAX +1)));
  common.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR);
  common.mvol.left = common.mvol.right = 0x3fff;
  SpuSetCommonAttr(&common);
}

void audio_load_voice(char *buffer, long size, u_long channel) {
  SpuVoiceAttr voiceAttr;
  u_long spuAddress;

  SpuSetTransferMode(SPU_TRANSFER_BY_IO);
  spuAddress = SpuMalloc(size); 
  SpuSetTransferStartAddr(spuAddress);
  SpuWrite(buffer, size);
  SpuIsTransferCompleted(SPU_TRANSFER_WAIT);

  voiceAttr.mask = (
    SPU_VOICE_VOLL |
    SPU_VOICE_VOLR |
    SPU_VOICE_PITCH |
    SPU_VOICE_WDSA |
    SPU_VOICE_ADSR_AMODE |
    SPU_VOICE_ADSR_SMODE |
    SPU_VOICE_ADSR_RMODE |
    SPU_VOICE_ADSR_AR |
    SPU_VOICE_ADSR_DR |
    SPU_VOICE_ADSR_SR |
    SPU_VOICE_ADSR_RR |
    SPU_VOICE_ADSR_SL
  );
  voiceAttr.voice = SPU_VOICECH(channel);
  voiceAttr.volume.left = voiceAttr.volume.right = 0x3fff;
  voiceAttr.pitch = 2048;
  voiceAttr.addr = spuAddress;

  voiceAttr.a_mode = SPU_VOICE_LINEARIncN;
  voiceAttr.s_mode = SPU_VOICE_LINEARIncN;
  voiceAttr.r_mode = SPU_VOICE_LINEARDecN;
  voiceAttr.ar = 0x0;
  voiceAttr.dr = 0x0;
  voiceAttr.sr = 0x0;
  voiceAttr.rr = 0x0;
  voiceAttr.sl = 0xf;

  SpuSetVoiceAttr(&voiceAttr);
}

void audio_play_voice(u_long channel) {
  SpuSetKey(SPU_ON, SPU_VOICECH(channel));
}

void audio_stop_voice(u_long channel) {
  SpuSetKey(SPU_OFF, SPU_VOICECH(channel));
}
