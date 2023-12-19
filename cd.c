#include "cd.h"

#define CD_SECTOR_SIZE 2048

void cd_init() {
  CdInit();
}

cd_File cd_load_file(char *filename) {
  cd_File result;
  CdlFILE file;
  int sectorsCount;
  //u_long *buffer;

  if(!CdSearchFile(&file, filename)) {
    printf("Couldn't find file %s\n", filename);
    return result;
  }

  sectorsCount = (file.size +  CD_SECTOR_SIZE - 1)/CD_SECTOR_SIZE;
  result.buffer = (char*)malloc3(SECTOR_SIZE * sectorsCount);
  printf("Reading file %s, %d sectors\n", filename, sectorsCount);
  
  CdControlB(CdlSetloc, (u_char*)&file.pos, NULL);
  //CdReadSync(0, NULL);
  CdRead(sectorsCount, (u_long*)result.buffer, CdlModeSpeed);
  CdReadSync(0, NULL);

  result.size = sectorsCount * CD_SECTOR_SIZE;
  return result;
}

void cd_free_file(cd_File file) {
  printf("Freeing 0x%08x\n", file.buffer);
  free3(file.buffer);
}
