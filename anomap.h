// MIT License
// Copyright (c) 2022 Anotra
// https://github.com/Anotra/anomap

#pragma once

#ifndef ANOMAP_H
#define ANOMAP_H

#include <stddef.h>
#include <stdbool.h>

#define ANOMAP_DECLARE_COMPARE_FUNCTION(function_name, data_type)             \
  static int                                                                  \
  function_name(const void *a, const void *b) {                               \
    if (*(data_type *)a == *(data_type *)b) return 0;                         \
    return *(data_type *)a > *(data_type *)b ? 1 : -1;                        \
  }

typedef enum {
#define ANOMAP_REVERSE_ORDER  anomap_reverse_order
  anomap_reverse_order        = 1 << 0,
#define ANOMAP_DIRECT_ACCESS  anomap_direct_access
  anomap_direct_access        = 1 << 1,
#define ANOMAP_PRESERVE_ORDER anomap_preserve_order
  anomap_preserve_order       = 1 << 2,
#define ANOMAP_USE_LOCK       anomap_use_lock
  anomap_use_lock             = 1 << 3,
} anomap_options;

typedef enum {
#define ANOMAP_INSERT anomap_insert
  anomap_insert = 1 << 0,
#define ANOMAP_UPDATE anomap_update
  anomap_update = 1 << 1,
#define ANOMAP_UPSERT anomap_upsert
  anomap_upsert = anomap_insert | anomap_update,
#define ANOMAP_DELETE anomap_delete
  anomap_delete = 1 << 2,
#define ANOMAP_GETVAL anomap_getval
  anomap_getval = 1 << 3,
} anomap_operation;

struct anomap;

anomap_options anomap_supported_options(void);

size_t anomap_struct_size(void);

bool anomap_init(struct anomap *map,
                 size_t key_size, size_t val_size,
                 int (*cmp)(const void *, const void *),
                 anomap_options options);

struct anomap *anomap_create(size_t key_size, size_t val_size,
                             int (*cmp)(const void *, const void *),
                             anomap_options options);
void anomap_destroy(struct anomap *map);

struct anomap_item_changed {
  struct anomap *map;
  void *data;
  anomap_operation op;
  void *key;
  struct {
    void *prev;
    void *now;
  } val;
};

typedef void anomap_on_item_changed(const struct anomap_item_changed *ev);

void anomap_set_on_item_changed(
  struct anomap *map, anomap_on_item_changed *on_changed, void *data);

size_t anomap_length(struct anomap *map);
void anomap_clear(struct anomap *map);

bool anomap_contains(struct anomap *map, void *key);
bool anomap_index_of(struct anomap *map, void *key, size_t *index);
bool anomap_at_index(struct anomap *map, size_t index, void *key, void *val);
const void *anomap_direct_key_at_index(struct anomap *map, size_t index);
void *anomap_direct_val_at_index(struct anomap *map, size_t index);

anomap_operation anomap_do(struct anomap *map,
                           anomap_operation operation,
                           void *key, void *val);

size_t anomap_copy_range(struct anomap *map,
                         size_t from_index, size_t to_index,
                         void *keys, void *vals);
size_t anomap_delete_range(struct anomap *map,
                           size_t from_index, size_t to_index,
                           void *keys, void *vals);

typedef void anomap_foreach_cb(struct anomap *map, void *data,
                               const void *key, const void *val);
void anomap_foreach(struct anomap *map, anomap_foreach_cb *cb, void *data);
void anomap_foreach_reverse(struct anomap *map,
                            anomap_foreach_cb *cb, void *data);

typedef enum {
#define ANOMAP_HEAD anomap_head
  anomap_head,
#define ANOMAP_TAIL anomap_tail
  anomap_tail,
#define ANOMAP_PREV anomap_prev
  anomap_prev,
#define ANOMAP_NEXT anomap_next
  anomap_next,
} anomap_position;

bool anomap_advance(struct anomap *map, 
                    size_t *index, 
                    anomap_position *position);

int anomap_cmp_str(const void *a, const void *b);

#endif // !ANOMAP_H
