#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MLD.h"
#include "object_db.h"
#include "my_structs.h"

int main()
{

  /*
    Start Register Possible structs / primitive ptrs to be used
  */
  structure_db_t *struct_db = calloc(1, sizeof(structure_db_t));

  static field_info_t emp_fields[] = {
      FIELD_INFO(emp_t, emp_id, UINT32, 0),
      FIELD_INFO(emp_t, emp_name, STRING, 0),
      FIELD_INFO(emp_t, age, UINT32, 0),
      FIELD_INFO(emp_t, salary, FLOAT, 0),
      FIELD_INFO(emp_t, mgr, OBJ_PTR, emp_t),
  };

  static field_info_t std_fields[] = {
      FIELD_INFO(student_t, stud_name, STRING, 0),
      FIELD_INFO(student_t, rollno, INT32, 0),
      FIELD_INFO(student_t, age, INT32, 0),
      FIELD_INFO(student_t, student_mark, INT32_PRIMITIVE_PTR, int *),
  };

  static field_info_t int_ptr_fields[] = {};

  REGISTER_STRUCTURE(struct_db, student_t, std_fields);
  REGISTER_STRUCTURE(struct_db, emp_t, emp_fields);
  REGISTER_STRUCTURE(struct_db, int *, int_ptr_fields);

  /*
    Finish Register Possible structs / primitive ptrs to be used
  */

  /*
     Start Allocating our objects using "xalloc"
  */

  object_db_t *obj_db = init_object_db(struct_db);
  student_t *stds1 = xalloc(obj_db, "student_t", 2);
  strcpy(stds1[0].stud_name, "ahmad");
  stds1[0].age = 20;
  stds1[0].rollno = 1;
  stds1[0].student_mark = NULL;

  strcpy(stds1[1].stud_name, "ali");
  stds1[1].age = 30;
  stds1[1].rollno = 2;
  stds1[1].student_mark = NULL;

  set_object_as_root(obj_db, stds1);

  student_t *std2 = xalloc(obj_db, "student_t", 1);
  strcpy(std2->stud_name, "test with primitive pointer");
  std2->age = 99;
  std2->rollno = 12;
  std2->student_mark = xalloc(obj_db, "int *", 1);
  *std2->student_mark = -1;
  set_object_as_root(obj_db, std2);

  emp_t *emp1 = xalloc(obj_db, "emp_t", 1);
  emp1->emp_id = 11;
  emp1->age = 20;
  strcpy(emp1->emp_name, "employee1");
  emp1->mgr = NULL;
  emp1->salary = 2000;

  emp_t *emp2 = xalloc(obj_db, "emp_t", 1);
  emp2->emp_id = 222;
  emp2->age = 33;
  strcpy(emp2->emp_name, "employee2");
  emp2->mgr = emp1;
  emp2->salary = 3000;
  set_object_as_root(obj_db, emp2);

  /*
     Finish Allocating our objects using "xalloc"
  */

  /*
      Start running our memory detector alogorithm
   */
  run_memory_detector_algorithm(obj_db);
  print_memory_report(obj_db);

  /*
      Finish running our memory detector alogorithm
   */

  return 0;
}