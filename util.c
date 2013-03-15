#include "util.h"

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

