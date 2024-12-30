typedef struct _student
{

  char stud_name[32];

  int rollno;

  int age;

  int *student_mark;

} student_t;

typedef struct emp_
{

  char emp_name[30];

  unsigned int emp_id;

  unsigned int age;

  struct emp_ *mgr;

  float salary;

} emp_t;