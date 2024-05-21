
#include "cache/cache.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc > 1) {
    printf("Usage: %s\n", argv[0]);
    return 1;
  }

  struct Cache *cache = read_cache();
  if (cache == NULL) {
    printf("Failed to read cache\n");
    return 1;
  }

  for (int i = 0; i < cache->pairs; i++) {
    printf("%s %s\n", cache->keys[i], cache->values[i]);
  }

  free_cache(cache);
  return 0;
}
