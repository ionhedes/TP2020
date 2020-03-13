#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Used for debugging and peace of mind
// At the end of execution, it should always equate 0
unsigned malloc_counter;

typedef enum {Undefined, Int, Double, Char, String}TypeList; // Represents types more consistently than numbers

typedef struct
{
  char * name;
  TypeList type;
  union
  {
    int decimal_number;
    double floating_point_number;
    char character;
    struct
    {
      char * content;
      unsigned length;
    }string;
  }data;
}Variable;

// Generic function for treating input strings and allocating them dinamically
char * getString()
{
  char * string = NULL, * aux = NULL;
  char buffer_char;
  unsigned length = 0;

  // Strings are considered to span until \n
  while ((buffer_char = getchar()) != '\n')
  {
    if ((aux = (char *)realloc(string, (length + 1) * sizeof(char))) == NULL)
    {
      printf("getString() error..\nUnreleased memory blocks: %d\n", malloc_counter);
      free(string);
      malloc_counter--;
      return NULL;
    }
    string = aux;

    // malloc_counter should only be increased once, on the first iteration
    malloc_counter += (!length ? 1 : 0);
    *(string + length) = buffer_char;
    length++;
  }

  // No use setting up the \0 character if we don't have a valid string
  if (string)
  {
    *(string + length) = '\0';
  }
  return string;
}

// Frees everything related to a variable
// As all the pointers in a struct SEEM to be initially set to NULL (analogous ints/doubles are set to 0),
// we can always check the existence of something before freeing it (makes the malloc_counter more reliable)
void freeVar(Variable * var)
{
  if (var->name)
  {
    free(var->name);
    malloc_counter--;
  }

  //If the type of the variable is a string, we have something else to free
  if (var->type == String && var->data.string.content)
  {
    free(var->data.string.content);
    malloc_counter--;
  }

  // var should always exist at this point, otherwise the execution would have stopped a long time ago
  free(var);
  malloc_counter--;
}

int checkString(char * string)
{
  // Basic conditions for the string to be considered valid include:
  // - the first character of the string must be a letter
  // - there must be an = sign inside the string
  if(!isalpha(string [0]) || !strchr(string, '='))
  {
    return 0;
  }
  return 1;
}

// Used for both integers and floating points, hence the void *
void extractNumber(void * destination, char * number_starting_point, char * point_position)
{
  int * integer_destination = NULL;
  double * floating_point_destination = NULL;
  char *aux = NULL;

  // Here we make the difference between the 2 data types
  if (point_position)
  {
    floating_point_destination = (double *)destination; // Setting the pointer back to its original type
    // Adding integer value
    for (aux = number_starting_point; isdigit(*aux); aux++)
    {
      *floating_point_destination = *floating_point_destination * 10 + (*aux - '0');
    }
    // Adding decimal value
    for(aux = point_position + 1; isdigit(*aux); aux++)
    {
      *floating_point_destination = *floating_point_destination + (*aux - '0') * pow(10, (double)(point_position - aux));
    }
  }
  else
  {
    integer_destination = (int *)destination; // Setting the pointer back to its original type
    // Adding integer value
    for (aux = number_starting_point; isdigit(*aux); aux++)
    {
      *integer_destination = *integer_destination * 10 + (*aux - '0');
    }
  }
}

// Used for strings, pretty self explanatory
void extractString(char * destination, char * string_starting_point, char * string_ending_point)
{
  int i = 0;
  char * aux = NULL;
  for (aux = string_starting_point + 1; aux < string_ending_point; aux++, i++)
  {
    *(destination + i) = *aux;
  }
  *(destination + i) = '\0';
}

// Parses the right-value, checks for errors, puts everything inside the var
// This accounts for the value of the variable
int parseRVal(Variable * var, char * string)
{
  int is_negative = 0;
  char alphabet [] = "abcdefghijklmnopqrstuvwxyz";
  char separators [] = "()[]{}\\|_;:,<.>/?`~!@#$%^&*\n";
  char * opening_bracket = NULL, * closing_bracket = NULL, * aux = NULL;
  char * goto_attribution = strchr(string, '='), * goto_rval = NULL, * goto_point = NULL, * goto_null = NULL;
  //printf("Found = at %ld\n", goto_attribution - string);


  goto_rval = goto_attribution + 1;
  //Gets rid of whitespaces
  while (*goto_rval == ' ')
  {
    goto_rval++;
  }
  //printf("Found rvalue at %ld\n", goto_rval - string);

  //Now we have the address where the rvalue should start, we start working on inferring a type
  if (strchr(separators, *goto_rval))
  {
    return 0; //Invalid input
  }

  // Detected " - might be string, checking...
  else if (*goto_rval == 34) // 34 = "
  {
    var->type = String;
    opening_bracket = goto_rval; // Found opening double-bracket
    if ((closing_bracket = strchr(opening_bracket + 1, 34)))
    {
      while((aux = strchr(closing_bracket + 1, 34))) // Found closing double-bracket, there might still be others
      {
        closing_bracket = aux;
      }
      // After the loop, we are sure to have the final closing double-bracket

      // Find the position of the \0 character
      goto_null = closing_bracket + 1;
      while (*goto_null == ' ') // Find the position of the \0 character
      {
        goto_null++;
      }
      if (*goto_null) // There should only be spaces after the variable value and the terminating character
      {
        return 0;
      }

      var->data.string.length = closing_bracket - opening_bracket - 1; //Set the length of the string

      if ((var->data.string.content = (char *)malloc((var->data.string.length + 1) * sizeof(char))) == NULL)
      {
        return 0;
      }
      malloc_counter++;

      extractString(var->data.string.content, opening_bracket, closing_bracket); // Puts the string in the var
    }
    else
    {
      return 0; // Invalid input
    }
  }

  // Detected ' - might be a char, checking...
  else if (*goto_rval == 39) // 39 = '
  {
    //printf("Found single opening bracket at %ld\n", goto_rval - string);
    var->type = Char;
    opening_bracket = goto_rval; // Found opening single-bracket
    closing_bracket = opening_bracket + 2; // Because we are dealing with a character, the closing single-bracket should be 2 characters away
    if (*(closing_bracket) != 39) // If it isn't, the input is invalid
    {
      return 0;
    }

    // Find the position of the \0 character
    goto_null = closing_bracket + 1;
    while(*goto_null == ' ')
    {
      goto_null++;
    }
    if (*goto_null) // There should only be spaces after the variable value and the terminating character
    {
      return 0;
    }
    var->data.character = *(opening_bracket + 1); //The char is in between the brackets, and can be anything
  }

  // Digit or minus detected - might be int/double, checking...
  else if (isdigit(*goto_rval) || *goto_rval == '-')
  {
    // Treating the case when the number is negative
    if (*goto_rval == '-')
    {
      // Flag that the number is negative
      is_negative = 1;
      while (*(++goto_rval) == ' ')
      {
        //Skipping possible whitespaces
      }
      // If the following char is anything but a number, the input is invalid
      if (!isdigit(*goto_rval))
      {
        return 0;
      }
    }
    // No non-number characters allowed
    if (strpbrk(goto_rval, alphabet) || strchr(goto_rval, 34) || strchr(goto_rval, 39))
    {
      return 0;
    }

    //We have a number, finding out whether it is an integer, or a floating point number
    if ((goto_point = strchr (goto_rval + 1, '.')))
    {
      //Fooling the program so I can check for invalid characters a lot easier
      *goto_point = '0';
      if (strpbrk(goto_rval, separators))
      {
        return 0;
      }
      *goto_point = '.';
      var->type = Double;

      // Just to be sure, we set it to 0
      var->data.floating_point_number = 0;
      extractNumber((void *)&(var->data.floating_point_number), goto_rval, goto_point);

      //If the number is negative, correct the sign
      if (is_negative)
      {
        var->data.floating_point_number = -var->data.floating_point_number;
      }
    }
    else
    {
      if (strpbrk(goto_rval, separators))
      {
        return 0;
      }
      var->type = Int;

      // Just to be sure, we set it to 0
      var->data.decimal_number = 0;
      extractNumber((void *)&(var->data.decimal_number), goto_rval, NULL);

      //If the number is negative, correct the sign
      if (is_negative)
      {
        var->data.decimal_number = -var->data.decimal_number;
      }
    }
  }

  // Anything else is wrong
  else
  {
    return 0;
  }

  return 1;
}

// Parses the left-value, checks for errors, puts everything inside the var
// This accounts for the name of the variable
int parseLVal(Variable * var, char * string)
{
  char * aux = NULL;
  char invalid_chararcters [] = " ()[]{}\\|;:,<.>/?`~!@#$%^&*\n";
  char * goto_end = strchr(string, '=') - 1, * goto_start = string;
  int length;

  // Get rid of whitespaces, both in front and after the variable name
  // Also compute the length of the variable name
  while (*goto_end == ' ')
  {
    goto_end--;
  }
  while (*goto_start == ' ')
  {
    goto_start++;
  }
  length = goto_end - goto_start - 1;

  for (aux = goto_start; aux <= goto_end; aux++)
  {
    // Check if there are invalid characters for a variable name
    if (strchr(invalid_chararcters, *aux))
    {
      return 0;
    }
  }

  if ((var->name = (char *)malloc((length + 1) * sizeof(char))) == NULL)
  {
    return 0;
  }
  malloc_counter++;

  // We call it like this (-1, +1) because of the structure of the loop inside extractString()
  // Prone to change in next release
  extractString(var->name, goto_start - 1, goto_end + 1);

  return 1;
}

int main()
{
  Variable * var = NULL;
  char * string = NULL;

  printf("Enter the desired attribution:\n");
  string = getString();
  // Checks for generic errors, and if the default conditions are met
  if (checkString(string))
  {
    //It is better to have the variable have its memory allocated here, so it doesn't snowball into messy code
    if ((var = (Variable *)malloc(sizeof(Variable))) == NULL)
    {
      printf("Something went wrong when making space for the variable.\nUnreleased memory blocks: %d;\nExiting...\n", malloc_counter);
      exit(EXIT_FAILURE);
    }
    malloc_counter++;

    // Parses both the left and right side of the expression.
    //  These functions also check for most of the errors that might appear
    if (!parseRVal(var, string) || !parseLVal(var, string))
    {
      // If any memory allocation, or invalid character occurs, the functions will throw 0 back at us, so we know to stop the execution
      freeVar(var);
      free(string);
      malloc_counter--;
      printf("Something went wrong, or the input is incorrect.\nUnreleased memory blocks: %d;\nExiting...\n", malloc_counter);
      exit(EXIT_FAILURE);
    }

    printf("The name is: %s\n", var->name);
    switch(var->type)
    {
      case Undefined:
      {
        printf("Warning, you shouldn't be here!\n");
        freeVar(var);
        break;
      }
      case Int:
      {
        printf("The integer is: %d\n", var->data.decimal_number);
        freeVar(var);
        break;
      }
      case Char:
      {
        printf("The caracter is: %c\n", var->data.character);
        freeVar(var);
        break;
      }
      case String:
      {
        printf("The string is: %s\n", var->data.string.content);
        freeVar(var);
        break;
      }
      case Double:
      {
        printf("The floating point number is: %f\n", var->data.floating_point_number);
        freeVar(var);
        break;
      }
    }
  }
  else
  {
    printf("Invalid input.\n");
  }

  free(string);
  malloc_counter--;
  printf("Unreleased memory blocks: %d;\nExiting...\n", malloc_counter);
  return 0;
}
