#pragma once

#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128

#define GET_FIELD_OFFSET(struct_name, field_name) (int)(&(((struct_name *)0)->field_name))
#define GET_FIELD_SIZE(struct_name, field_name) sizeof(((struct_name *)0)->field_name)
#define FIELD_INFO(structure_name, field_name, datatype, nested_struct_name)                                 \
  {                                                                                                          \
      #field_name, GET_FIELD_SIZE(structure_name, field_name), GET_FIELD_OFFSET(structure_name, field_name), \
      datatype,                                                                                              \
      #nested_struct_name}

#define REGISTER_STRUCTURE(structure_db, structure_name, fields_info)    \
  do                                                                     \
  {                                                                      \
    struct_db_record_t *rec = malloc(sizeof(struct_db_record_t));        \
    strncpy(rec->struct_name, #structure_name, MAX_STRUCTURE_NAME_SIZE); \
    rec->data_structure_size = sizeof(structure_name);                   \
    rec->number_of_fields = sizeof(fields_info) / sizeof(field_info_t);  \
    rec->fields = fields_info;                                           \
    rec->next = NULL;                                                    \
    add_structure_db(structure_db, rec);                                 \
  } while (0)
;

typedef enum
{
  UINT8,
  UINT32,
  INT32,
  CHAR,
  STRING,
  OBJ_PTR,
  FLOAT,
  DOUBLE,
  OBJ_STRUCT,
} field_type_t;

typedef struct field_info_
{
  char field_name[MAX_FIELD_NAME_SIZE];
  unsigned int size;
  unsigned int offset;
  field_type_t datatype;
  char nested_structure_name[MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

typedef struct struct_db_record_
{
  char struct_name[MAX_STRUCTURE_NAME_SIZE];
  unsigned int data_structure_size;
  unsigned int number_of_fields;
  field_info_t *fields;
  struct struct_db_record_ *next;

} struct_db_record_t;

typedef struct structure_db
{
  struct_db_record_t *head;
  unsigned int count;

} structure_db_t;

void print_structure_db(structure_db_t *structure_db);
void print_structure_record(struct_db_record_t *structure_record);
void print_field_info_t(field_info_t *field_info, unsigned int number_of_fields);
void add_structure_db(structure_db_t *structure_db, struct_db_record_t *structure_record);
struct_db_record_t *struct_db_look_up(structure_db_t *struct_db, char *struct_name);
