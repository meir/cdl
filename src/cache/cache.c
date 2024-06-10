
#include "cache.h"

void init_cache_file() {
  // create cache file if it doesn't exist
  FILE *file = fopen(get_cache_filepath(), "r");
  if (file == NULL) {
    file = fopen(get_cache_filepath(), "w");
  }
  if (file != NULL) {
    fclose(file);
  }
}

char *get_cache_filepath() {
  const char *home = getenv("HOME");
  if (home != NULL) {
    char *filepath = malloc(strlen(home) + strlen("/.cache/cdlist") + 1);
    if (filepath != NULL) {
      strcpy(filepath, home);
      strcat(filepath, "/.cache/cdlist");
      return filepath;
    }
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

  char key[MAX_LINE_LENGTH];
  char value[MAX_LINE_LENGTH];
  int scans;
  while ((scans = fscanf(file, "%[^ ] %s\n", key, value)) != EOF) {
    if (scans == 2 && cache->pairs < MAX_PATHS) {
      cache->keys[cache->pairs] = strdup(key);
      cache->values[cache->pairs] = strdup(value);
      if (cache->keys[cache->pairs] == NULL ||
          cache->values[cache->pairs] == NULL) {
        // Handle allocation error
        fclose(file);
        free_cache(cache);
        return NULL;
      }
      cache->pairs++;
    }
  }

  fclose(file);
  return cache;
}

void write_cache(struct Cache *cache) {
  init_cache_file();

  FILE *file = fopen(get_cache_filepath(), "w");
  if (file == NULL) {
    printf("Failed to open cache file\n");
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
  free(cache);
}
