#include <stdio.h>

#include "args.h"
#include "config.h"
#include "monitor.h"

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
  configuration* config;
  if (config_file) {
    config = parse_config(config_file);
  } else {
    fprintf(stderr, "Error: no configuration file specified? (see -c flag)\n");
    return -1;
  }

  printf("log file: %s\n", config->log);
  process_configuration *p_config = config->process_config;
  int i;
  while (p_config) {
    printf("\nProcess: %s\n", p_config->name);
    printf("  cd: %s\n", p_config->cd);
    printf("  exec: %s\n", p_config->exec);
    printf("  pid_file: %s\n", p_config->pid_file);
    printf("  args: ");
    for(i=0; p_config->args[i]; i++) {
      printf("%s ", p_config->args[i]);
      fflush(stdout);
    }
    printf("\n");
    p_config = p_config->next_process;
  }

  monitor_processes(config);

  return 0;
}
