
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
  for (int i = 0; i < cache->pairs; i++) {
    if (strcmp(cache->keys[i], name) == 0) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    printf("No such entry in list\n");
    free_cache(cache);
    return 1;
  }

  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    printf("Failed to get current working directory\n");
    free_cache(cache);
    return 1;
  }

  // remove index from cache
  if (index < cache->pairs) {
    free(cache->keys[index]);
    free(cache->values[index]);
    for (int i = index; i < cache->pairs - 1; i++) {
      cache->keys[i] = cache->keys[i + 1];
      cache->values[i] = cache->values[i + 1];
    }
    cache->pairs--;
  }

  write_cache(cache);

  free_cache(cache);
  return 0;
}
