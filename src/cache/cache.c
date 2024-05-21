
#include "cache.h"

void init_cache_file() {
  // create cache file if it doesn't exist
  FILE *file = fopen(get_cache_filepath(), "r");
  if (file == NULL) {
    file = fopen(get_cache_filepath(), "w");
  }
  fclose(file);
}

char *get_cache_filepath() {
  const char *home = getenv("HOME");
  if (home != NULL) {
    return strcat(strcat(strdup(home), "/"), ".cache/cdlist");
  }
  return NULL;
}

struct Cache *read_cache() {
  init_cache_file();

  FILE *file = fopen(get_cache_filepath(), "r");
  if (file == NULL) {
    return NULL;
  }

  struct Cache *cache = malloc(sizeof(struct Cache));
  if (cache == NULL) {
    fclose(file);
    return NULL;
  }

  cache->pairs = 0;

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file) != NULL) {
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];
    if (sscanf(line, "%[^ ] %s", key, value) == 2) {
      cache->keys[cache->pairs] = strdup(key);
      cache->values[cache->pairs] = strdup(value);
      cache->pairs++;
    } else {
      continue;
    }
  }

  return cache;
}

void write_cache(struct Cache *cache) {
  init_cache_file();

  FILE *file = fopen(get_cache_filepath(), "w");
  if (file == NULL) {
    printf("Failed to open cache file\n");
    fclose(file);
    return;
  }

  for (int i = 0; i < cache->pairs; i++) {
    fprintf(file, "%s %s\n", cache->keys[i], cache->values[i]);
  }

  fclose(file);
}

void free_cache(struct Cache *cache) {
  for (int i = 0; i < cache->pairs; i++) {
    free(cache->keys[i]);
    free(cache->values[i]);
  }
}
