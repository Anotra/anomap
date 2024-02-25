#include <stdlib.h>
#include <stdio.h>
#include "anomap.h"

ANOMAP_DECLARE_COMPARE_FUNCTION(_cmp_int, int)

int
test_ordering(void) {
  struct anomap *map = anomap_create(sizeof (int), 0,
                          _cmp_int, anomap_preserve_order);

  for (int i=0; i<100; i+=2)
    anomap_do(map, anomap_insert, &i, NULL);
  for (int i=1; i<100; i+=2)
    anomap_do(map, anomap_insert, &i, NULL);

#define DO_ADVANCE_AND_CHECK                      \
  do {                                            \
    if (!anomap_advance(map, &index, &p))         \
      return 1;                                   \
    int key;                                      \
    if (!anomap_at_index(map, index, &key, NULL)) \
      return 2;                                   \
    if (key != i)                                 \
      return 3;                                   \
  } while (0)

  size_t index;
  anomap_position p;
  { //forward
    p = anomap_head;
    for (int i=0; i<100; i+=2)
      DO_ADVANCE_AND_CHECK;
    for (int i=1; i<100; i+=2)
      DO_ADVANCE_AND_CHECK;
    if (anomap_advance(map, &index, &p))
      return 4;
  }
  { //reverse
    p = anomap_tail;
    for (int i=99; i>=0; i-=2)
      DO_ADVANCE_AND_CHECK;
    for (int i=98; i>=0; i-=2)
      DO_ADVANCE_AND_CHECK;
    if (anomap_advance(map, &index, &p))
      return 4;
  }
  { //check order of indicies
    for (size_t i=0; i<anomap_length(map); i++) {
      int key;
      anomap_at_index(map, i, &key, NULL);
      if (key != i) return 5;
    }
  }
  return 0;
}