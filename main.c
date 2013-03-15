#include <stdio.h>

#include "args.h"
#include "config.h"

/*
 * main function
 */
int main(int argc, char** argv) {
  //daemonize();

  /*FILE *fp = NULL;*/
  /*fp = fopen("/Users/bjohnson/Desktop/daemon.log", "a");*/
  /*while(1) {*/
    /*sleep(1);*/
    /*_log(fp, "Daemon logging");*/
    /*fflush(fp);*/
  /*}*/

  char* config_file = config_file_path();
  if (config_file) {
    configuration* config = parse_config(config_file);
  } else {
    fprintf(stderr, "Error: no configuration file specified? (see -c flag)\n");
    return -1;
  }

  return 0;
}

