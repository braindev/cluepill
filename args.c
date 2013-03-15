#include "args.h"

char* config_file_path(int argc, char** argv) {
  int ch;

  while ((ch = getopt(argc, argv, "c:")) != -1) {
    switch(ch) {
      case'c':
        return strdup(optarg);
        break;
    }
  }

  return NULL;
}
