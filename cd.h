#ifndef CD_H
#define CD_H

#include <sys/types.h>
#include <kernel.h>
#include <libcd.h>
#include "types.h"

typedef struct cd_File {
  char *buffer;
  u_long size;
} cd_File;

void cd_init();
cd_File cd_load_file(char *filename);
void cd_free_file(cd_File file);

#endif
