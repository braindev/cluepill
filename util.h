#ifndef _CLUEPILL_UTIL
#define _CLUEPILL_UTIL

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char* read_file(char* filename);
void _log(FILE *fp, char *message);
void daemonize();

#endif
