#ifndef __CLUEPILL_MONITOR
#define __CLUEPILL_MONITOR

#include <errno.h>
#include <signal.h>
#include <string.h>

#include "config.h"
#include "util.h"

void monitor_processes(configuration *config);
int spawn_process(process_configuration *p_config);

#endif
