#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "anomap.h"

ANOMAP_DECLARE_COMPARE_FUNCTION(_cmp_int, int)

static struct anomap *map;
#define ITERATIONS 100000
static void *
_routine(void *arg) {
  struct anomap *local_map = anomap_create(sizeof (int), 0, _cmp_int, 0);
  for (int i=0; i < ITERATIONS; i++) {
    if (anomap_do(map, anomap_insert, &i, NULL))
      anomap_do(local_map, anomap_insert, &i, NULL);
  }
  return local_map;
}

int
test_locks(void) {
  if (0 == (anomap_supported_options() & anomap_use_lock))
    return 1;
  map = anomap_create(sizeof (int), 0, _cmp_int, anomap_use_lock);
#define THREAD_COUNT 4
  pthread_t threads[THREAD_COUNT];
  for (int i=0; i<THREAD_COUNT; i++)
    pthread_create(threads + i, NULL, _routine, NULL);

  size_t total_len = 0;
  for (int i=0; i<THREAD_COUNT; i++) {
    void *local_map;
    pthread_join(threads[i], &local_map);
    total_len += anomap_length(local_map);
  }
  printf("%zu %zu\n", total_len, anomap_length(map));
  if (total_len != ITERATIONS)
    return 2;
  if (anomap_length(map) != ITERATIONS)
    return 3;
  return 0;
}