#include "config.h"
#include "util.h"

/*
 * parse and return the configuration given the path to a json configuration file
 */
configuration* parse_config(char* config_file) {
  int i, j;
  process_configuration *previous_process_config = NULL, *process_config = NULL;

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
    config->log = strdup(jv_log->u.string.ptr);
  }

  json_value* jv_processes = value_for_key(jv, "processes");
  json_value* json_str;
  json_value* process_args;
  if (jv_processes && jv_processes->type == json_array) {
    for(i=0; i<jv_processes->u.array.length; i++) {
      process_config = calloc(sizeof(process_configuration), 1);
      process_config->name =  strdup(value_for_key(jv_processes->u.array.values[i], "name")->u.string.ptr);
      process_config->cd = strdup(value_for_key(jv_processes->u.array.values[i], "cd")->u.string.ptr);
      process_config->exec = strdup(value_for_key(jv_processes->u.array.values[i], "exec")->u.string.ptr);
      process_config->pid_file = strdup(value_for_key(jv_processes->u.array.values[i], "pid_file")->u.string.ptr);
      process_config->stderr_file = strdup(value_for_key(jv_processes->u.array.values[i], "stderr_file")->u.string.ptr);
      process_config->stdout_file = strdup(value_for_key(jv_processes->u.array.values[i], "stdout_file")->u.string.ptr);
      json_str = value_for_key(jv_processes->u.array.values[i], "start");
      process_config->start = json_str->u.boolean ? 1 : 0;
      process_args = value_for_key(jv_processes->u.array.values[i], "args");
      if (process_args && process_args->type == json_array) {
        process_config->args = malloc((sizeof(char *) * process_args->u.array.length) + 1);
        process_config->args[process_args->u.array.length + 1] = NULL;
        for(j=0; j<process_args->u.array.length; j++) {
          process_config->args[j] = strdup(process_args->u.array.values[j]->u.string.ptr);
        }
      }
      if (previous_process_config) {
        previous_process_config->next_process = process_config;
      } else {
        config->process_config = process_config;
      }
      previous_process_config = process_config;
    }
  } else {
    return NULL;
  }

  free(config_str);
  json_value_free(jv);

  return config;
}

/*
 * returns the json object for a given string key
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
