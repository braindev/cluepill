#include "monitor.h"

/*
 *
 */
void monitor_processes(configuration *config) {
  process_configuration *p_config;
  pid_t check_pid;
  int check_ret;

  while(1) {
    p_config = config->process_config;
    while (p_config) {
      char *pid_str = read_file(p_config->pid_file);
      if (pid_str) {
        check_pid = atoi(pid_str);
        check_ret = kill(check_pid, 0);
        if (check_ret != 0 && errno == ESRCH) {
          printf("no such pid %d?\n", check_pid);
          spawn_process(p_config);
        }
      } else {
        // TODO
        printf("no pid file %s?\n", p_config->pid_file);
        spawn_process(p_config);
      }
      p_config = p_config->next_process;
    }
    sleep(1);
    printf("*"); fflush(stdout);
  }
}

/*
 *
 */
int spawn_process(process_configuration *p_config) {

  pid_t pid, pid2, sid;
  int exec_ret;

  pid = fork();
  if (pid == -1) {
    // fork failed
    return -1;
  } else if (pid == 0) {
    // child process
    umask(0); //unmask the file mode
    sid = setsid(); // set new session
    if(sid < 0) {
      exit(1);
    }
    close(STDIN_FILENO); close(STDOUT_FILENO); close(STDERR_FILENO);
    chdir("/");
    pid2 = fork();
    if (pid2 == -1) {
      // fork failed
      exit(1);
    } else if (pid2 == 0) {
      // child of child
      chdir(p_config->cd);
      exec_ret = execvp(p_config->exec, p_config->args);
    } else {
      // child
      exit(0);
    }
    if (exec_ret != 0) {
      exit(1);
    }
  } else {
    // parent process
    return 0;
  }

  return -1;
}
