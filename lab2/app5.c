#include <stdio.h>
#include <stdlib.h>

// Used for debugging and peace of mind
// At the end of execution, it should always equate 0
unsigned malloc_counter;

typedef struct
{
  unsigned char danger_level:2;
  unsigned char recommended_minimum_age:5;
  unsigned char need_prescription:1;
}Medicine;

// Turns the danger level code of the medicine into a string
void extractDangerLevel(unsigned char value)
{
  char * danger_levels [] = {"not dangerous/insignificantly dangerous", "commonly dangerous", "highly dangerous"};
  printf("%s", danger_levels [value]);
}

void getMedicineData(Medicine * medicine)
{
  // Some auxiliary variables, since we can't scanf() directly into a bit-field
  unsigned aux_danger_level, aux_recommended_minimum_age, aux_need_prescription; // Making the numbers unsigned int, instead of unsigned char before storing them
                                                                                 // When using the values in comparisons, it is safer to do it like this (lest exceptions may occur)
                                                                                 // Before storing the values in the structure, we cast it to unsigned char

  printf("Do you need prescription for this medicine? (1/0) ");
  scanf("%u", &aux_need_prescription);
  if (aux_need_prescription != 0 && aux_need_prescription != 1) // Can either be 1 - yes, we need prescription, or 0 - no, we don't need prescription
  {
    printf("WARNING, you are trying to input a number incompatible with this field!\n");
    do {
      printf("Please try again with a correct number: ");
      scanf("%u", &aux_need_prescription);
    } while(aux_need_prescription != 0 && aux_need_prescription != 1); // Inputting new choices until we find the one that fits
  }
  medicine->need_prescription = (unsigned char)aux_need_prescription;

  printf("Enter the minimum recommended age for administering this medicine: ");
  scanf("%u", &aux_recommended_minimum_age);
  if (aux_recommended_minimum_age > 18) // Smaller than 18
  {
    printf("WARNING, you are trying to input a number incompatible with this field!\n");
    do {
      printf("Please try again with a correct number: ");
      scanf("%u", &aux_recommended_minimum_age);
    } while(aux_recommended_minimum_age > 18); // Inputting new choices until we find the one that fits
  }
  medicine->recommended_minimum_age = (unsigned char)aux_recommended_minimum_age;

  printf("Enter the danger level possibly caused by administering this medicine: ");
  scanf("%u", &aux_danger_level);
  if (aux_danger_level != 0 && aux_danger_level != 1 && aux_danger_level != 2) // Can either be:
                                                                               // 0 - not dangerous
                                                                               // 1 - commonly dangerous
                                                                               // 2 - highly dangerous
  {
    printf("WARNING, you are trying to input a number incompatible with this field!\n");
    do {
      printf("Please try again with a correct number: ");
      scanf("%u", &aux_danger_level);
    } while(aux_danger_level != 0 && aux_danger_level != 1 && aux_danger_level != 2); // Inputting new choices until we find the one that fits
  }
  medicine->danger_level = (unsigned char)aux_danger_level;
}

void printMedicineInfo(Medicine * medicine)
{
  printf("Details about the medicine:\n");
  printf("\t - prescription is ");
  if (medicine->need_prescription)
  {
    printf("needed;\n");
  }
  else
  {
    printf("not needed;\n");
  }
  printf("\t - minimum recommended age for administering: %d;\n", medicine->recommended_minimum_age);
  printf("\t - danger level of the medicine: ");
  extractDangerLevel(medicine -> danger_level);
  printf(";\n");
}

Medicine * createMedicine()
{
  Medicine * medicine = NULL;

  // Make space for the medicine structure variable
  if ((medicine = (Medicine *)malloc(sizeof(Medicine))) == NULL)
  {
    return NULL;
  }
  malloc_counter++;

  // The measurement structure gets filled
  getMedicineData(medicine);

  return medicine;
}


int main()
{
  Medicine * medicine = NULL;

  if ((medicine = createMedicine()) == NULL)
  {
    fprintf(stderr, "Failed to create Medicine object;\nUnreleased memory blocks: %d;\nExiting...", malloc_counter);
    exit(EXIT_FAILURE);
  }

  putchar('\n');
  printMedicineInfo(medicine);
  putchar('\n');

  free(medicine);
  malloc_counter--;
  printf("Unreleased memory blocks: %d;\nExiting...\n", malloc_counter);
  return 0;
}
