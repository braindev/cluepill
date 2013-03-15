#ifndef _CLUEPILL_CONFIG
#define _CLUEPILL_CONFIG

#include <string.h>
#include <unistd.h>

#include "json.h"

typedef struct {
  pid_t pid;
  char* name;
  char* cd;
  char* exec;
  char** args;
  int argc;
  unsigned char start;
  char* pid_file;
  void* next_process;
} process_configuration;

typedef struct {
  char* log;
  process_configuration* process_config;
} configuration;

configuration* parse_config(char* config_file);
json_value *value_for_key(json_value* jo, char* key);

#endif
