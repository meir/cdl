
#include "cache/cache.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *name = "last";
  if (argc > 2) {
    printf("Usage: %s (name)\n", argv[0]);
    return 1;
  } else if (argc == 2) {
    name = argv[1];
  }

  struct Cache *cache = read_cache();
  if (cache == NULL) {
    printf("Failed to read cache\n");
    return 1;
  }

  int index = -1;
  for (index = 0; index < cache->pairs; index++) {
    if (strcmp(cache->keys[index], name) == 0) {
      break;
    }
  }

  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    printf("Failed to get current working directory\n");
    free_cache(cache);
    return 1;
  }

  cache->keys[index] = strdup(name);
  cache->values[index] = strdup(cwd);
  if (index == cache->pairs) {
    cache->pairs++;
  }
  write_cache(cache);

  free_cache(cache);
  return 0;
}
