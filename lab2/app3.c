#include <stdio.h>
#include <stdlib.h>

unsigned malloc_counter;

// The size of a struct where there are bitfields is still an integer number of bytes
// We can still use malloc() to declare them
typedef struct
{
  unsigned leg_num:10;
  unsigned is_dangerous:1;
  unsigned life_expectancy:11;
  char scientific_code [9];
  float weight;
}Animal;

void getAnimalData(Animal * animal)
{
  // Since we are using bit fields, with no address, we need auxiliary variables to store the data,
  // before we store it into the structure's fields
  unsigned aux_leg_num, aux_is_dangerous, aux_life_expectancy;

  // Only the bit-field variables of the structure need to be handled like this
  // Variables declared with normal data types their size can still be accessed via their addresses

  // There are some decisional structures in place before storing the bit-field values,
  // so they don't overflow the variable

  // Scientific code - normal string
  printf("Enter the scientific code of the animal: ");
  scanf("%8s", animal->scientific_code);

  // Life expectancy - shrunk unsigned
  printf("Enter the maximum life expectancy of the animal: ");
  scanf("%u", &aux_life_expectancy);
  if (aux_life_expectancy > 2047)
  {
    printf("WARNING, you are trying to input a number greater than the admitted limit for this field!\n");
    do {
      printf("Please try again with a correct number: ");
      scanf("%u", &aux_life_expectancy);
    } while(aux_life_expectancy > 2047);
  }
  animal->life_expectancy = aux_life_expectancy;

  // Leg number - shrunk unsigned
  printf("Enter the leg number of the animal: ");
  scanf("%u", &aux_leg_num);
  if (aux_leg_num > 1023)
  {
    printf("WARNING, you are trying to input a number greater than the admitted limit for this field!\n");
    do {
      printf("Please try again with a correct number: ");
      scanf("%u", &aux_leg_num);
    } while(aux_leg_num > 1023);
  }
  animal->leg_num = aux_leg_num;

  // Weight - normal float
  printf("Enter the usual weight of the animal: ");
  scanf("%f", &(animal->weight));

  // Dangerous flag - shrunk unsigned (acts as a bool)
  printf("Is the animal dangerous for humans? (1/0) ");
  scanf("%u", &aux_is_dangerous);
  if (aux_is_dangerous > 1)
  {
    printf("WARNING, you are trying to input a number greater than the admitted limit for this field!\n");
    do {
      printf("Please try again with a correct number: ");
      scanf("%u", &aux_is_dangerous);
    } while(aux_is_dangerous > 1);
  }
  animal->is_dangerous = aux_is_dangerous;
}

void printAnimalInfo(Animal * animal)
{
  printf("Animal #%s:\n", animal->scientific_code);
  printf("\t - lives a maximum of %d years;\n", animal->life_expectancy);
  printf("\t - has an usual weight of %.2f kilograms;\n", animal->weight);
  printf("\t - has a number of %d legs;\n", animal->leg_num);
  if (animal->is_dangerous)
  {
    printf("\t - the animal is dangerous for humans;\n");
  }
  else
  {
    printf("\t - is not dangerous for humans;\n");
  }
  putchar('\n');
}

Animal * createAnimal()
{
  Animal * animal = NULL;
  
  // Make space for the animal structure
  if ((animal = (Animal *)malloc(sizeof(Animal))) == NULL)
  {
    return NULL; // Letting main() deal with reporting the error to the user, and execution stopping
  }
  malloc_counter ++;

  // The animal structure gets filled
  getAnimalData(animal);

  return animal;
}

int main()
{
  Animal * animal = NULL;
  if ((animal = createAnimal()) == NULL)
  {
    fprintf(stderr, "Failed to create Animal object;\nUnreleased memory blocks: %d;\nExiting...", malloc_counter);
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printAnimalInfo(animal);
  free(animal);
  malloc_counter--;
  printf("Unreleased memory blocks: %d;\nExiting...\n", malloc_counter);
  return 0;
}
