#pragma once
#include <stdbool.h>
#include "MLD.h"

typedef struct object_db_record_
{
  void *objcet_ptr;
  unsigned int number_of_units; // number of allocated units (for example in case we have used calloc())
  struct object_db_record_ *next;
  struct_db_record_t *struct_info;
  bool is_visited;
  bool is_root;

} object_db_record_t;

typedef struct object_db_
{
  object_db_record_t *object_db_head;
  structure_db_t *structure_db;
  int count;
} object_db_t;

object_db_t *init_object_db(structure_db_t *structure_db);

void *xalloc(object_db_t *obj_db, char *structure_name, int units);
void xfree(object_db_t *obj_db, void *ptr);

void register_global_object_as_root(structure_db_t *struct_db, object_db_t *obj_db, void *object_ptr, char *struct_name, unsigned int number_of_units);
void set_object_as_root(object_db_t *obj_db, void *object_ptr);

void add_object_db_record(object_db_t *obj_db, object_db_record_t *obj_db_record);
object_db_record_t *object_db_record_lookup(object_db_t *obj_db, void *object_address);

void run_memory_detector_algorithm(object_db_t *obj_db);

void print_object_db(object_db_t *obj_db);
void print_object_record_db(object_db_record_t *obj_db_record);
void traverse_record_db_objects(object_db_t *obj_db, object_db_record_t *obj_db_record, int indentation);

void print_memory_report(object_db_t *obj_db);