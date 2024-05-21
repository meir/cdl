#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATHS 256
#define MAX_LINE_LENGTH 1024

struct Cache {
  char *keys[MAX_PATHS];
  char *values[MAX_PATHS];

  int pairs;
};

void init_cache_file();
char *get_cache_filepath();

struct Cache *read_cache();
void write_cache(struct Cache *cache);
void free_cache(struct Cache *cache);
