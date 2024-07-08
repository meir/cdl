
#include "cache/cache.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

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

  for (int index = 0; index < cache->pairs; index++) {
    // Check if paths exist and delete any that do not
    struct stat st;
    char *path = cache->values[index];
    if (stat(path, &st) == -1) {
      free(cache->keys[index]);
      free(cache->values[index]);
      for (int i = index; i < cache->pairs - 1; i++) {
        cache->keys[i] = cache->keys[i + 1];
        cache->values[i] = cache->values[i + 1];
      }
      cache->pairs--;
    }

    write_cache(cache);
  }

  free_cache(cache);
  return 0;
}
