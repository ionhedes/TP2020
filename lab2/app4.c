#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <math.h>
#define MAX_VAL 63
#define ORDER_OFFSET 12
#define ACCEPTED_DECIMAL_ERROR pow(10, -7) // Because of the inaccurate nature of floating point number calculations, we need an acceptable error to make the computation feasible
#define TIME_OUT_SWITCH 40 // Used when a while loop goes berserk

// WARNING!
// Because of the memory limitation imposed by this exercise, and of the incompatibility between floating point data types and bit fields,
// the problem can only set multiples, not submultiples

// Used for debugging and peace of mind
// At the end of execution, it should always equate 0
unsigned malloc_counter;

typedef struct
{
  unsigned char magnitude:8;
  unsigned char unit:2;
  unsigned char order:5; // Using an excess-6 for the offset of the order of magnitude
}Measurement;

// Finds the unit of measurement, by matching it against some patterns
int checkUnit(char * unit_to_match, char * unit_to_check)
{
  char regex_error_buffer [81]; // Used for debugging if the regex execution fails
  int return_checker; // Checker for the return value of matching functions
  regex_t regex; // A regular expression object

  // Compiling the regular expression:
  // Compilation flags:
  // - REG_EXTENDED - uses extended syntax (thought this would add some useful symbols to the syntax, but it didn't)
  // - REG_NOSUB - when matching, the regular expression won't return the position of the matches - we simply don't need it
  // - REG_ICASE - the regular expression will be case-insensitive
  if (regcomp(&regex, unit_to_match, REG_EXTENDED | REG_NOSUB | REG_ICASE))
  {
    return -1; // As opposed to malloc(), calloc(), etc..., regcomp() and regexec() returns non-zero value when it encounters an error
  }

  return_checker = regexec(&regex, unit_to_check, 0, NULL, REG_NOTBOL);

  // As opposed to malloc(), calloc(), etc..., regcomp() and regexec() returns non-zero value when it encounters an error
  if (!return_checker)
  {
    regfree(&regex);
    return 1; // Match found
  }
  else if (return_checker == REG_NOMATCH)
  {
    regfree(&regex);
    return 0; // No match found
  }
  else
  {
    regerror(return_checker, &regex, regex_error_buffer, sizeof(regex_error_buffer)); // Transforms the returned code of regexec() into an error text and puts it into a bufer
    fprintf(stderr, "Regex match failed: %s\n", regex_error_buffer);
    regfree(&regex);
    return -1; // Error encountered
  }

  regfree(&regex); // Decompile/free the regex
  return -1; // If the execution reaches this point, it is same to assume that we've encountered an error
}

// Sets the right scale order of the measurement, while adjusting the magnitude correspondingly
// Keeps the precision of the measurement
void normaliseMagnitude(Measurement * measurement, float * aux_magnitude)
{
  unsigned loop_counter = 0;
  int integer_part;
  float fractional_part = *aux_magnitude - floor(*aux_magnitude); // Calculating the integer part of our value
  measurement->order = ORDER_OFFSET;

  // If the number has a fractional part, we need to scale down the order (we will save the information is submultiples of our unit)
  // If the number has no fractional part and can be divided by 10, we need to scale up the order (we will save the information is multiples of our unit)

  if (fractional_part < ACCEPTED_DECIMAL_ERROR) // If the fractional part is smaller than the error, we consider it to be zero
  {
    // The computations are made considering the value is an integer
    // The computations are made using an auxiliary integer
    integer_part = (int)(*aux_magnitude);
    while (integer_part % 10 == 0 && integer_part) // If the integer_part is 0, we need to stop the loop, or else it breaks the program
    {
      *aux_magnitude /= 10;
      integer_part /= 10;
      measurement->order++;
    }
  }
  else if (fractional_part > 1 - ACCEPTED_DECIMAL_ERROR) // If the fractional number is very close to one, we approximate the value by adding 1
  {
    // The computations are made considering the value is an integer
    // The computations are made using an auxiliary integer
    integer_part = (int)(*aux_magnitude + 1);
    *aux_magnitude = ceil(*aux_magnitude);
    while (integer_part % 10 == 0 && integer_part) // If the integer_part is 0, we need to stop the loop, or else it breaks the program
    {
      *aux_magnitude /= 10;
      integer_part /= 10;
      measurement->order++;
    }
  }
  else // In any other case, we have a valid fractional value
  {
    // This loop is prone to get stuck, so we need to implement an failsafe
    while (fractional_part > ACCEPTED_DECIMAL_ERROR && fractional_part < 1 - ACCEPTED_DECIMAL_ERROR) // we do this until the fractional part escapes these boundaries
    {
      loop_counter++;
      if(loop_counter > TIME_OUT_SWITCH)
      {
        malloc_counter--;
        free(measurement);
        fprintf(stderr, "The computation of the normalised magnitude timed out (caused by an infinite loop and floating point operation inaccuracies);\nUnreleased memory blocks: %d;\nExiting...\n", malloc_counter);
        exit(EXIT_FAILURE);
      }
      *aux_magnitude *= 10;
      fractional_part = *aux_magnitude - floor(*aux_magnitude);
      measurement->order--;
    }
  }
}

// Checks if the scale order is set to something invalid (like a quintal)
int isInvalidOrder(const unsigned order)
{
  return (order == 1 || order == 2 || order == 4 || order == 5 || order == 7 || order == 8 || order == 16 || order == 17 || order == 19 || order == 20 || order == 22 || order == 23);
}

// Turns the order code of the measurement into a string
void extractMultiple(const unsigned order)
{
  // Warning, we have no scale order for some multiples of 10
  char * multiple_list [] = {"Pico", "", "", "Nano", "", "", "Micro", "", "", "Mili", "Centi", "Deci", "", "Deca", "Hecto", "Kilo", "", "", "Mega", "", "", "Giga", "", "", "Tera"};

  // An array starts with index 0, hence we cannot use negative indexes for submultiples - we use an offset
  printf("%s", multiple_list [order]);
}

// Turns the unit code of the measurement into a string
void extractUnit(const unsigned unit)
{
  char * unit_list [] = {"gram", "liter", "meter"};
  printf("%s", unit_list [unit]);
}

void getMeasurementData(Measurement * measurement)
{
  // Some auxiliary variables, since we can't scanf() directly into a bit-field
  float aux_magnitude;
  char aux_unit [11];
  int i, return_checker, found = 0;

  // These regular expressions are overly-simplified, because I don't get how to use them in C
  // To look into another regex library at a later date
  // This is how I would have wanted them to look -> ^g(ram)?$
  // - explanation : math a word that starts with "g" and ends with 0 of 1 groups of "ram"
  char * unit_list [] = {"gram", "liter", "(meter)|(metre)"}; // Regexes to be matched

  printf("Enter the measurement unit: ");
  scanf("%10s", aux_unit);

  // We check the input string against the accepted patterns
  for (i = 0; i < 3; i++)
  {
    return_checker = checkUnit(unit_list [i], aux_unit);
    if (return_checker == 1)
    {
      found = 1; // When found, we flag it so we know we can continue execution
      measurement->unit = i;
      break;
    }
    else if (return_checker == -1) // If the checkUnit() function fails, we stop the execution
    {
      // Error message already printed by checkUnit()
      free(measurement);
      malloc_counter--;
      exit(EXIT_FAILURE);
    }
  }

  if (!found) // If the unit wasn't found, we stop the execution
  {
    free(measurement);
    malloc_counter--;
    printf("The measurement unit is invalid;\nUnreleased memory blocks: %d;\nExiting...", malloc_counter);
    exit(EXIT_FAILURE);
  }

  printf("Enter the value/magnitude: ");
  scanf("%f", &aux_magnitude);
  normaliseMagnitude(measurement, &aux_magnitude); // After getting the magnitude/value of the measurement, we send it to scale order correction

  // Checking if the corrected magnitude fits the field in the struct, and if the scale order is recognized
  if (aux_magnitude > MAX_VAL || isInvalidOrder(measurement->order))
  {
    printf("WARNING, you are trying to input a number incompatible with this field!\n");
    do {
      printf("Please try again with a correct number: ");
      scanf("%f", &aux_magnitude);
      normaliseMagnitude(measurement, &aux_magnitude);
    } while(aux_magnitude > MAX_VAL || isInvalidOrder(measurement->order)); // Inputting new magnitudes until we find the one that fits
  }
  measurement->magnitude = (unsigned)aux_magnitude;
}

void printMeasurementInfo(Measurement * measurement)
{
  printf("Details about the measurement:\n\t - measurement unit: ");
  extractMultiple(measurement->order);
  extractUnit(measurement->unit);
  printf(";\n");
  printf("\t - measurement value: %d;\n", measurement->magnitude);
}

Measurement * createMeasurement()
{
  Measurement * measurement = NULL;

  // Make space for the measurement structure
  if ((measurement = (Measurement *)malloc(sizeof(Measurement))) == NULL)
  {
    return NULL;
  }
  malloc_counter++;

  // The measurement structure gets filled
  getMeasurementData(measurement);

  return measurement;
}

int main()
{
  Measurement * measurement = NULL;

  if ((measurement = createMeasurement()) == NULL)
  {
    fprintf(stderr, "Failed to create Measurement object;\nUnreleased memory blocks: %d;\nExiting...", malloc_counter);
    exit(EXIT_FAILURE);
  }

  putchar('\n');
  printMeasurementInfo(measurement);
  putchar('\n');

  free(measurement);
  malloc_counter--;
  printf("Unreleased memory blocks: %d;\nExiting...\n", malloc_counter);
  return 0;
}
