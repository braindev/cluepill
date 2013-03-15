#include "config.h"

/*
 * main function
 */
int main(int argc, char** argv, char** envp) {
  //daemonize();

  /*FILE *fp = NULL;*/
  /*fp = fopen("/Users/bjohnson/Desktop/daemon.log", "a");*/
  /*while(1) {*/
    /*sleep(1);*/
    /*_log(fp, "Daemon logging");*/
    /*fflush(fp);*/
  /*}*/

  configuration* config = parse_config("/Users/bjohnson/Desktop/cluepill/cluepill_conf.json");

  return 0;
}

