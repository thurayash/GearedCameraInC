#ifndef INIT_H_
#define INIT_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <gmodule.h>
#include <glib.h>
#include <poll.h>

#include "json_parser.h"

#define BUF_SIZE 256
#define MAX_MATCHES 10
#define TIMEOUT 100

#define COLOR_WHITE     "\033[0;37m"
#define COLOR_RED       "\033[0;31m"
#define COLOR_BLUE      "\033[0;34m"
#define COLOR_YELLOW    "\033[0;33m"
#define COLOR_CYAN      "\033[0;36m"
#define COLOR_GREEN     "\033[0;32m"

#endif
