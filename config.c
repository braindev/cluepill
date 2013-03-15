#include "config.h"
#include "util.h"

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
