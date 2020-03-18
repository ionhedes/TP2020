#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Used for debugging and peace of mind
// At the end of execution, it should always equate 0
unsigned malloc_counter;

typedef struct
{
  char * name;
  float mark;
}Student;

void freeEvalArray(Student ** eval_array, int size)
{
  int i;

  // Freeing all already-set variables/ frees all if end of input has been reached
  for (i = 0; i < size; i++)
  {
    // Only frees name if it was set - it is set on null by default
    if (eval_array [i]->name)
    {
      free(eval_array [i]->name);
      malloc_counter--;
    }
    free(eval_array [i]);
    malloc_counter--;
  }
  free(eval_array);
  malloc_counter--;
}

// Function that compares the marks of the students
int compare(const void *a, const void *b)
{
  // Our student structures are referenced by pointers, stored in a pointer array
  // We need to cast the void pointers to Student ** pointers
  Student ** a_aux = (Student **)a;
  Student ** b_aux = (Student **)b;

  // Dereferencing the array pointer to get to the structure pointer
  // Then dereferencing again, and accesing the structure
  if ((*a_aux)->mark > (*b_aux)->mark)
  {
    return -1;
  }
  else if ((*a_aux)->mark < (*b_aux)->mark)
  {
    return 1;
  }
  else
  {
    return strcmp((*a_aux)->name, (*b_aux)->name);
  }
}

// Gets string char-by-char, and stores them into a dinamically-allocated string
// Returns pointer to the string
// Handles its own allocation errors, reports back to main, which terminates the program
char * getString()
{
  char * string = NULL, * aux = NULL;
  char buffer_char;
  unsigned length = 0;

  // Strings are considered to span until \n
  while ((buffer_char = getchar()) != '\n')
  {
    if ((aux = (char *)realloc(string, (length +  1) * sizeof(char))) == NULL)
    {
      fprintf(stderr, "getString() error..\nUnreleased memory blocks: %d\n", malloc_counter);
      free(string);
      malloc_counter--;
      return NULL;
    }
    string = aux;

    //malloc_counter should only be increased once, on the first iteration
    malloc_counter += (!length ? 1 : 0);
    *(string + length) = buffer_char;
    length++;
  }

  // No use setting up the \0 character if we don't have a valid string
  if (string)
  {
    *(string+length) = '\0';
  }
  return string;
}

void printStudents(Student ** eval_array, int n)
{
  int i;
  printf("Student list:\n");
  for(i = 0; i < n; i++)
  {
    printf("\t - %d: %s, %.2f\n", i + 1, eval_array [i]->name, eval_array [i]->mark);
  }
}

// Creates the array with students and marks
// Only makes space for the data structure, doesn't fill the pointers
// They are filled while reading
Student ** initEvalArray(int student_number)
{
  Student ** eval_array = NULL;

  if ((eval_array = (Student **)malloc(student_number * sizeof(Student *))) == NULL)
  {
    return NULL;
  }
  malloc_counter++;

  return eval_array;
}

// Creates new individual entry in the array
Student * createStudent()
{
  Student * student = NULL;

  if ((student = (Student *)malloc(sizeof(Student))) == NULL)
  {
    return NULL;
  }
  malloc_counter++;

  student->name = NULL; // Just to make sure this pointer initially points to nothing

  return student;
}

// Function to fill in a Student structure
int getStudentData(Student * student)
{
  printf("\t - enter the name: ");
  if ((student->name = getString()) == NULL)
  {
    return 0;
  }

  printf("\t - enter the mark: ");
  scanf("%f", &(student->mark));
  if (student->mark < 0 || student->mark > 10) // Checking if the input mark is valid
  {
    printf("WARNING, you are trying to input a number incompatible with this field!\n");
    do {
      printf("Please try again with a correct number: ");
      scanf("%f", &student->mark);
    } while(student->mark < 0 || student->mark > 10); // Inputting new choices until we find the one that fits
  }

  putchar('\n');
  return 1;
}

int main()
{
  // Pointer to a comparison function
  int (*compare_function)(const void *, const void *);
  // Attributing the pointer
  compare_function = compare;

  int n, i;
  Student ** eval_array = NULL;

  // Initializing the whole array with marks and names
  printf("Enter the number of students who have taken the exam: ");
  scanf("%d", &n);
  if ((eval_array = initEvalArray(n)) == NULL)
  {
    fprintf(stderr, "Something went wrong when creating the evaluation array.\nUnreleased memory blocks: %d;\nExiting...\n", malloc_counter);
    exit(EXIT_FAILURE);
  }
  putchar('\n');

  // Filling in the data
  for (i = 0; i < n; i++)
  {
    getchar();
    if ((eval_array [i] = createStudent()) == NULL)
    {
      fprintf(stderr, "Something went wrong when adding student #%d.\nUnreleased memory blocks: %d;\nExiting...\n", i, malloc_counter);
      freeEvalArray(eval_array, i);
      exit(EXIT_FAILURE);
    }
    printf("Student %d:\n", i + 1);
    if (!getStudentData(eval_array [i]))
    {
      freeEvalArray(eval_array, i);
      exit(EXIT_FAILURE);
    }
  }

  // Printing the students, unsorted
  printStudents(eval_array, n);

  // Sort and print the "leaderboard"
  qsort(eval_array, n, sizeof(Student *), compare_function);
  printf("Sorting...\n");
  printStudents(eval_array, n);
  putchar('\n');


  freeEvalArray(eval_array, n);
  printf("Unreleased memory blocks: %d;\nExiting...\n", malloc_counter);
  return 0;
}
