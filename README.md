# anomap
sorted key/value storage for C99

## Example Usage
```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "anomap.h"

// declare compare function
ANOMAP_DECLARE_COMPARE_FUNCTION(_cmp_int, int)

static void
on_changed(const struct anomap_item_changed *ev) {
  if (ev->op & anomap_insert)
    printf("inserted [%i] = '%s'\n", *(int *)ev->key, *(char **)ev->val.now);

  if (ev->op & anomap_update) {
    printf("updated [%i] = '%s' to '%s'\n",
                                  *(int *)ev->key,
                                  *(char **)ev->val.prev,
                                  *(char **)ev->val.now);
    free(*(char **)ev->val.prev);
  }

  if (ev->op & anomap_delete) {
    printf("deleted [%i] = '%s'\n", *(int *)ev->key, *(char **)ev->val.prev);
    free(*(char **)ev->val.prev);
  }
}

int main() {
    struct anomap *map = anomap_create(
        sizeof(int),    //< key_size
        sizeof(char *), //< val_size
        _cmp_int,       //< cmp_func
        0               //< options
      );

    // we don't need this, but register for events
    anomap_set_on_item_changed(map, on_changed, "user data");

    // insert 10 items
    for (int key = 0; key < 10; key++) {
      char *val = calloc(128, sizeof *val);
      sprintf(val, "value for key %i.", key);
      anomap_do(map, anomap_insert, &key, &val);
    }

    // update 5 items
    for (int key = 0; key < 5; key++) {
      char *val = calloc(128, sizeof *val);
      sprintf(val, "updated value for key %i.", key);
      anomap_do(map, anomap_update, &key, &val);
    }

    // find an item
    int key = 3;
    char *val = NULL;
    if (anomap_do(map, anomap_getval, &key, &val) & anomap_getval)
      printf("FOUND: [%i] = '%s'\n", key, val);

    // delete a range, copying keys
    int keys[8];
    anomap_delete_range(map, 1, 8, keys /* can be NULL */, NULL);
    for (int i = 0; i < 8; i++)
      printf("array from delete_range: %i\n", keys[i]);

    // iterate over all items
    for (size_t i = 0; i < anomap_length(map); i++) {
      int key;
      anomap_at_index(map, i, &key, NULL);
      printf("iterating: %i\n", key);
    }

    // clear the map of all items
    anomap_clear(map);

    // destroy the map when done
    anomap_destroy(map);
}

```
