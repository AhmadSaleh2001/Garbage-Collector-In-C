#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>
#include "MLD.h"
#include "object_db.h"
#include "my_structs.h"

object_db_t *init_object_db(structure_db_t *structure_db)
{
  object_db_t *obj_db = malloc(sizeof(object_db_t));
  obj_db->count = 0;
  obj_db->object_db_head = NULL;
  obj_db->structure_db = structure_db;
  return obj_db;
}

void *xalloc(object_db_t *obj_db, char *structure_name, int units)
{
  struct_db_record_t *object_struct = struct_db_look_up(obj_db->structure_db, structure_name);
  assert(object_struct != NULL);

  void *allocated_objects = calloc(units, object_struct->data_structure_size);

  object_db_record_t *db_record = calloc(1, sizeof(object_db_record_t));
  db_record->objcet_ptr = allocated_objects;
  db_record->number_of_units = units;
  db_record->struct_info = object_struct;
  db_record->next = NULL;
  db_record->is_root = false;
  db_record->is_visited = false;

  add_object_db_record(obj_db, db_record);

  return allocated_objects;
}

void xfree(object_db_t *obj_db, void *ptr)
{
  assert(ptr);

  object_db_record_t *curr = obj_db->object_db_head;
  object_db_record_t *prev = NULL;
  while (curr)
  {

    if (curr->objcet_ptr == ptr)
    {
      if (prev)
      {
        prev->next = curr->next;
      }
      else
      {
        obj_db->object_db_head = obj_db->object_db_head->next;
      }
      free(curr->objcet_ptr);
      free(curr);
      return;
    }

    prev = curr;
    curr = curr->next;
  }

  assert(0);
}

void register_global_object_as_root(structure_db_t *struct_db, object_db_t *obj_db, void *object_ptr, char *struct_name, unsigned int number_of_units)
{
  object_db_record_t *record = malloc(sizeof(object_db_record_t));
  record->objcet_ptr = object_ptr;
  record->number_of_units = number_of_units;
  record->next = NULL;
  record->struct_info = struct_db_look_up(struct_db, struct_name);
  record->is_root = true;
  record->is_visited = false;
  assert(record->struct_info);

  add_object_db_record(obj_db, record);
}

void set_object_as_root(object_db_t *obj_db, void *object_ptr)
{
  assert(obj_db);

  object_db_record_t *curr = obj_db->object_db_head;
  while (curr)
  {
    if (curr->objcet_ptr == object_ptr)
    {
      curr->is_root = true;
      return;
    }

    curr = curr->next;
  }
}

void add_object_db_record(object_db_t *obj_db, object_db_record_t *obj_db_record)
{
  if (!obj_db->object_db_head)
  {
    obj_db->object_db_head = obj_db_record;
  }
  else
  {
    obj_db_record->next = obj_db->object_db_head;
    obj_db->object_db_head = obj_db_record;
  }
  obj_db->count++;
}

object_db_record_t *object_db_record_lookup(object_db_t *obj_db, void *object_address)
{
  object_db_record_t *curr = obj_db->object_db_head;
  while (curr)
  {
    struct_db_record_t *struct_info = curr->struct_info;
    int step_size = struct_info->data_structure_size;

    for (int i = 0; i < curr->number_of_units; i++)
    {
      void *current_object_pointer = (void *)(curr->objcet_ptr + i * step_size);
      if (current_object_pointer == object_address)
      {
        return curr;
      }
    }

    curr = curr->next;
  }

  return NULL;
}

void run_memory_detector_algorithm(object_db_t *obj_db)
{
  printf("######### Running MLD algorithm #########\n");

  object_db_record_t *curr = obj_db->object_db_head;
  while (curr)
  {
    if (!curr->is_visited && curr->is_root)
    {
      traverse_record_db_objects(obj_db, curr, 0);
    }
    curr = curr->next;
    printf("\n");
  }

  printf("######### Running MLD algorithm #########\n");
}

void print_object_db(object_db_t *obj_db)
{
  printf("######### Printing object db structure #########\n");

  object_db_record_t *curr = obj_db->object_db_head;
  while (curr)
  {
    if (!curr->is_visited)
    {
      traverse_record_db_objects(obj_db, curr, 0);
    }
    curr = curr->next;
    printf("\n");
  }

  printf("######### Printing object db structure #########\n");
}

void print_object_record_db(object_db_record_t *obj_db_record)
{
  printf("\nObject ptr: %p\n", obj_db_record->objcet_ptr);
  printf("\tNumber of units: %d\n", obj_db_record->number_of_units);
  print_structure_record(obj_db_record->struct_info);
}

void traverse_record_db_objects(object_db_t *object_db, object_db_record_t *obj_db_record, int indentation)
{
  if (obj_db_record->is_visited)
    return;
  obj_db_record->is_visited = true;
  struct_db_record_t *struct_info = obj_db_record->struct_info;
  int step_size = struct_info->data_structure_size;

  for (int i = 0; i < obj_db_record->number_of_units; i++)
  {
    void *data_as_bytes = (char *)(obj_db_record->objcet_ptr + i * step_size);
    print_data_from_field_info(object_db, struct_info->fields, struct_info->number_of_fields, data_as_bytes, indentation);
  }
  // obj_db_record->is_visited = false;
}

void print_indentation(int indentation)
{
  for (int i = 0; i < indentation; i++)
  {
    printf(" ");
  }
}

void print_data_from_field_info(object_db_t *obj_db, field_info_t *field_info, unsigned int number_of_fields, void *data, int indentation)
{
  int cnt = 0;

  print_indentation(indentation);
  printf("Object address: %p\n", data);
  while (cnt < number_of_fields)
  {
    int offset = field_info[cnt].offset;
    print_indentation(indentation);
    switch (field_info[cnt].datatype)
    {
    case UINT8:
      printf("field name: %s, field data: %u\n", field_info[cnt].field_name, *((uint8_t *)(data + offset)));
      break;
    case UINT32:
      printf("field name: %s, field data: %u\n", field_info[cnt].field_name, *((uint32_t *)(data + offset)));
      break;
    case INT32:
      printf("field name: %s, field data: %d\n", field_info[cnt].field_name, *((int32_t *)(data + offset)));
      break;
    case CHAR:
      printf("field name: %s, field data: %c\n", field_info[cnt].field_name, *((char *)(data + offset)));
      break;
    case STRING:
      printf("field name: %s, field data: %s\n", field_info[cnt].field_name, ((char *)(data + offset)));
      break;
    case FLOAT:
      printf("field name: %s, field data: %f\n", field_info[cnt].field_name, *((float *)(data + offset)));
      break;
    case DOUBLE:
      printf("field name: %s, field data: %f\n", field_info[cnt].field_name, *((double *)(data + offset)));
      break;
    case INT32_PRIMITIVE_PTR:
    {
      void *nested_object_field_offset = (data + offset);
      char *nested_structure_name = field_info[cnt].nested_structure_name;
      unsigned int field_size = field_info[cnt].size;
      void *nested_object_address = NULL;
      memcpy(&nested_object_address, nested_object_field_offset, sizeof(void *));

      printf("field name: %s, field data: %p, nested struct name: %s\n", field_info[cnt].field_name, nested_object_address, nested_structure_name);
      if (nested_object_address != NULL)
      {
        object_db_record_t *nested_object_record = object_db_record_lookup(obj_db, nested_object_address);
        if (nested_object_record)
        {
          print_indentation(indentation + 1);
          printf("value: %d\n", *((int *)nested_object_record->objcet_ptr));
          traverse_record_db_objects(obj_db, nested_object_record, indentation + 1);
        }
      }
    }

    break;

    case OBJ_PTR:

    {
      void *nested_object_field_offset = (data + offset);
      char *nested_structure_name = field_info[cnt].nested_structure_name;
      unsigned int field_size = field_info[cnt].size;
      void *nested_object_address = NULL;
      memcpy(&nested_object_address, nested_object_field_offset, sizeof(void *));

      printf("field name: %s, field data: %p, nested struct name: %s\n", field_info[cnt].field_name, nested_object_address, nested_structure_name);
      if (nested_object_address != NULL)
      {
        object_db_record_t *nested_object_record = object_db_record_lookup(obj_db, nested_object_address);
        if (nested_object_record)
        {
          traverse_record_db_objects(obj_db, nested_object_record, indentation + 1);
        }
      }
    }

      break;
    }
    cnt++;
  }
}

void print_memory_report(object_db_t *obj_db)
{

  printf("######### Memory Report #########\n");
  object_db_record_t *curr = obj_db->object_db_head;
  while (curr)
  {
    if (!curr->is_visited)
    {
      printf("Item with struct name %s and address %p, is not deallocated\n", curr->struct_info->struct_name, curr->objcet_ptr);
    }

    curr = curr->next;
  }
  printf("######### Memory Report #########\n");
}