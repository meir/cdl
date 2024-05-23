
#include "cache/cache.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *name = "last";
  if (argc > 2) {
    printf("Usage: %s\n", argv[0]);
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
      printf("%s\n", cache->values[i]);
      free_cache(cache);
      return 0;
    }
  }

  free_cache(cache);
  return 1;
}
