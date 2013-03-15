#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include "json.h"

typedef struct {
  pid_t pid;
  char* name;
  char* cd;
  char* exec;
  char** args;
  int argc;
  unsigned char start;
  void* next_process;
} process;

typedef struct {
  char* log;
  process* processes;
} configuration;

void daemonize();
void _log(FILE *fp, char *message);
char* read_file(char* filename);
configuration* parse_config(char* config_file);
json_value *value_for_key(json_value* jo, char* key);

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

/*
 * read contents of a file and return the result as a char*
 * returns NULL on failure
 */
char* read_file(char* filename) {
  FILE *fp = NULL;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  rewind(fp);
  char* content = calloc(size + 1, 1);
  fread(content, 1, size, fp);
  fclose(fp);

  return content;
}

/*
 *
 */
configuration* parse_config(char* config_file) {
  int i, j;
  char* config_str = read_file(config_file);
  if (config_str == NULL) {
    return NULL;
  }

  configuration* config = calloc(sizeof(configuration), 1);
  json_value *jv = json_parse(config_str);
  if (jv == NULL || jv->type != json_object) {
    return NULL;
  }

  json_value* jv_log = value_for_key(jv, "log");
  if (jv_log && jv_log->type == json_string) {
    printf("log file: %s\n", jv_log->u.string.ptr);
    config->log = strdup(jv_log->u.string.ptr);
  }

  json_value* jv_processes = value_for_key(jv, "processes");
  json_value* json_str;
  json_value* process_args;
  if (jv_processes && jv_processes->type == json_array) {
    for(i=0; i<jv_processes->u.array.length; i++) {
      json_str = value_for_key(jv_processes->u.array.values[i], "name");
      printf("process name: %s\n", json_str->u.string.ptr);
      json_str = value_for_key(jv_processes->u.array.values[i], "cd");
      printf("process cd: %s\n", json_str->u.string.ptr);
      json_str = value_for_key(jv_processes->u.array.values[i], "exec");
      printf("process exec: %s\n", json_str->u.string.ptr);
      json_str = value_for_key(jv_processes->u.array.values[i], "start");
      printf("process start: %s\n", json_str->u.boolean ? "yes" : "no");
      process_args = value_for_key(jv_processes->u.array.values[i], "args");
      if (process_args && process_args->type == json_array) {
        printf("process args:\n");
        for(j=0; j<process_args->u.array.length; j++) {
          json_str = process_args->u.array.values[j];
          printf("  %s\n", json_str->u.string.ptr);
        }
      }
      printf("\n");
    }
  }



  free(config_str);

  return config;
}

/*
 *
 */
json_value *value_for_key(json_value* jo, char* key) {
  int i;
  for(i=0; i < jo->u.object.length; i++) {
    if (strcmp(jo->u.object.values[i].name, key) == 0) {
      return jo->u.object.values[i].value;
    }
  }
  return NULL;
}

/*
 * Log a message to a stream
 */
void _log(FILE *fp, char *message) {
  time_t current_time;
  struct tm* ltime;

  current_time = time(NULL);
  ltime = localtime(&current_time);

  fprintf(
      fp,
      "[%d-%02d-%02d %02d:%02d:%02d %s] %s",
      ltime->tm_year + 1900,
      ltime->tm_mon + 1,
      ltime->tm_mday,
      ltime->tm_hour,
      ltime->tm_min,
      ltime->tm_sec,
      ltime->tm_zone,
      message
  );
  fprintf(fp, "\n");
  fflush(fp);
}

/*
 * Daemonize the process
 */
void daemonize() {
  pid_t process_id = 0;
  pid_t sid = 0;

  // Create child process
  process_id = fork();
  if (process_id < 0) {
    printf("fork failed!\n");
    exit(1);
  }

  // PARENT PROCESS. Need to exit it.
  if (process_id > 0) {
    printf("process_id of child process %d \n", process_id);
    exit(0);
  }

  umask(0); //unmask the file mode

  sid = setsid(); // set new session
  if(sid < 0) {
    exit(1);
  }

  chdir("/");

  // Close stdin. stdout and stderr
  close(STDIN_FILENO); close(STDOUT_FILENO); close(STDERR_FILENO);
}
