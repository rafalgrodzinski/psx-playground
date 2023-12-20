#include "cd.h"

#define CD_SECTOR_SIZE 2048

void cd_init() {
  CdInit();
}

cd_File cd_load_file(char *filename) {
  cd_File result;
  CdlFILE file;
  int sectors_count;

  if(!CdSearchFile(&file, filename)) {
    printf("Couldn't find file %s\n", filename);
    return result;
  }

  sectors_count = (file.size +  CD_SECTOR_SIZE - 1)/CD_SECTOR_SIZE;
  result.size = CD_SECTOR_SIZE * sectors_count;
  result.buffer = (char*)malloc3(result.size);
  printf("Allocated 0x%x bytes at 0x08x\n", result.size, result.buffer);

  printf("Reading file %s, 0x%x bytes\n", filename, file.size);
  CdControl(CdlSetloc, (u_char*)&file.pos, NULL);
  CdRead(sectors_count, (u_long*)result.buffer, CdlModeSpeed);
  CdReadSync(0, NULL);

  return result;
}

void cd_free_file(cd_File file) {
  printf("Freeing 0x%x bytes at 0x%08x\n", file.size, file.buffer);
  free3(file.buffer);
}
