#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "MLD.h"
#include "my_structs.h"

void print_structure_db(structure_db_t *structure_db)
{
  printf("######### Printing db structure #########\n");
  struct_db_record_t *curr = structure_db->head;
  while (curr)
  {
    print_structure_record(curr);
    curr = curr->next;
  }
  printf("######### Printing db structure #########\n");
}

void print_structure_record(struct_db_record_t *structure_record)
{
  printf("Structure name: %s\n", structure_record->struct_name);
  printf("Structure size: %d\n", structure_record->data_structure_size);
  printf("Number of fields: %d\n", structure_record->number_of_fields);
  print_field_info_t(structure_record->fields, structure_record->number_of_fields);

  // please write field info
}

void print_field_info_t(field_info_t *field_info, unsigned int number_of_fields)
{
  int cnt = 0;
  while (cnt < number_of_fields)
  {

    printf("\tfield name: %s\n", field_info[cnt].field_name);
    printf("\tfield size: %d\n", field_info[cnt].size);
    printf("\tfield offset: %d\n", field_info[cnt].offset);
    printf("\tfield datatype: %d\n", field_info[cnt].datatype);
    if (field_info[cnt].datatype == OBJ_PTR || field_info[cnt].datatype == OBJ_STRUCT)
    {
      printf("\tfield nested structure name: %s\n", field_info[cnt].nested_structure_name);
    }

    cnt++;
  }
}

void add_structure_db(structure_db_t *structure_db, struct_db_record_t *structure_record)
{
  if (!structure_db->head)
  {
    structure_db->head = structure_record;
  }
  else
  {
    structure_record->next = structure_db->head;
    structure_db->head = structure_record;
  }

  structure_db->count++;
}

struct_db_record_t *struct_db_look_up(structure_db_t *struct_db, char *struct_name)
{
  struct_db_record_t *curr = struct_db->head;
  while (curr)
  {
    if (strcmp(curr->struct_name, struct_name) == 0)
    {
      return curr;
    }

    curr = curr->next;
  }
  return NULL;
}