//Always use getchar() inbetween using scanf() for decimal/floating point values and calling getString()
//Residual '\n' is not consumed by scanf()

#include <stdio.h>
#include <stdlib.h>

typedef enum {True, False} bool;
typedef enum {Gasoline, Diesel, Electric} EngineType;
typedef enum {Personal, Freight, Special} VehicleType;

unsigned malloc_counter;

typedef struct
{
  VehicleType veh;
  EngineType eng;
  char * brand;

  union
  {
    struct
    {
      unsigned int capacity, airbag_num;
    }pers;
    struct
    {
      double capacity;
      bool is_refrigerated;
    }fr;
    struct
    {
      char * custom_veh_type;
    }spcl;
  }specifier;
}
Vehicle;

void freeMem(Vehicle ** garage, const unsigned * vehicle_qnty)
{
  int i;
  if(garage)
  {
    for (i = 0; i < *vehicle_qnty; i++)
    {
      free(garage [i]->brand);
      if (garage [i]->veh == 2)
      {
        free(garage [i]->specifier.spcl.custom_veh_type);
        malloc_counter--;
      }
      free(garage [i]);
      malloc_counter -= 2;
    }
    free(garage);
    malloc_counter--;
  }
}

char * getString()
{
  char * string = NULL, * aux = NULL;
  char buffer_char;
  unsigned length = 0;

  while((buffer_char = getchar()) != '\n')
  {
    if ((aux = (char *)realloc(string, (length + 1) * sizeof(char))) == NULL)
    {
      printf("getString() error..\nUnreleased memory blocks: %d;\nExiting.\n", malloc_counter);
      free(string);
      return NULL;
    }
    string = aux;
    malloc_counter += (!length ? 1 : 0);
    *(string + length) = buffer_char;
    length++;
  }
  if (string)
  {
    *(string + length) = '\0';
  }
  return string;


}

void getVehicleData(Vehicle ** garage, const unsigned * vehicle_number)
{
  char placeholder;

  printf("Enter the vehicle type: ");
  scanf("%hhd", &placeholder);
  garage [*vehicle_number]->veh = (VehicleType)placeholder;
  printf("Enter the engine type: ");
  scanf("%hhd", &placeholder);
  garage [*vehicle_number]->eng = (EngineType)placeholder;

  //necessary for scanf() to work
  getchar();

  printf("Enter the vehicle's manufacturer name: ");
  if((garage [*vehicle_number]->brand = getString()) == NULL)
  {
    freeMem(garage, vehicle_number);
    exit(EXIT_FAILURE);
  }

  switch(garage [*vehicle_number]->veh)
  {
    case 0:
    {
      printf("Enter the personal vehicle's human capacity: ");
      scanf("%ud", &(garage [*vehicle_number]->specifier.pers.capacity));
      printf("Enter the personal vehicle's airbag number: ");
      scanf("%ud", &(garage [*vehicle_number]->specifier.pers.airbag_num));
      break;
    }
    case 1:
    {
      printf("Enter the freight vehicle's capacity: ");
      scanf("%lf", &(garage [*vehicle_number]->specifier.fr.capacity));
      printf("Is the vehicle refrigerated?(1/0) ");
      scanf("%hhd", &placeholder);
      garage [*vehicle_number]->specifier.fr.is_refrigerated = (bool)placeholder;
      break;
    }
    case 2:
    {
      printf("What type of vehicle is this? ");
      if ((garage [*vehicle_number]->specifier.spcl.custom_veh_type = getString()) == NULL)
      {
        freeMem(garage, vehicle_number);
        exit(EXIT_FAILURE);
      }
    }
  }
}

Vehicle ** addVehicle(Vehicle ** garage, unsigned * vehicle_qnty)
{
  Vehicle ** membuf = NULL;

  if ((membuf = (Vehicle **)realloc(garage, (*vehicle_qnty + 1) * sizeof(Vehicle *))) == NULL)
  {
    freeMem(garage, vehicle_qnty);
    printf("Adding vehicle %d failed..\nUnreleased memory blocks: %d;\nExiting.\n", *vehicle_qnty, malloc_counter);
    exit(EXIT_FAILURE);
  }
  malloc_counter += (*vehicle_qnty == 0 ? 1 : 0);
  garage = membuf;

  if ((garage [*vehicle_qnty] = (Vehicle *)malloc(sizeof(Vehicle))) == NULL)
  {
    freeMem(garage, vehicle_qnty);
    printf("Adding vehicle %d failed..\nUnreleased memory blocks: %d;\nExiting.\n", *vehicle_qnty, malloc_counter);
    exit(EXIT_FAILURE);
  }
  malloc_counter++;
  getVehicleData(garage, vehicle_qnty);
  (*vehicle_qnty)++;

  return garage;
}

void printVehicleReference()
{
  printf("Vehicle codes:\n");
  printf("0. Personal vehicle;\n");
  printf("1. Freight vehicle;\n");
  printf("2. Special purpose vehicle;\n");
  putchar('\n');
  printf("Engine codes:\n");
  printf("0. Gasoline engine;\n");
  printf("1. Diesel engine;\n");
  printf("2. Electric engine;\n");
  putchar('\n');
}

void generateMenu()
{
  printf("MENU\n");
  printf("0. Print reference chart for vehicle and engine types;\n");
  printf("1. Add vehicle;\n");
  printf("2. Print the list of vehicles;\n");
  printf("3. Exit;\n");
  putchar('\n');
  printf("What would you like to do? ");
}

void printVehicles(Vehicle ** garage, const unsigned * vehicle_qnty)
{
  int i;
  for (i = 0; i < *vehicle_qnty; i++)
  {
    printf(" - Vehicle %d: \n", i);
    printf("\t - brand: %s;\n", garage [i]->brand);
    switch (garage [i]->veh)
    {
      case 0:
      {
        printf("\t - vehicle type: personal;\n");
        printf("\t - capacity: %d;\n", garage [i]->specifier.pers.capacity);
        printf("\t - airbag number: %d;\n", garage [i]->specifier.pers.airbag_num);
        break;
      }
      case 1:
      {
        printf("\t - vehicle type: freight;\n");
        printf("\t - capacity: %.2f kgs;\n", garage [i]->specifier.fr.capacity);
        printf("\t - the vehicle is refrigerated: %d;\n", garage [i]->specifier.fr.is_refrigerated);
        break;
      }
      case 2:
      {
        printf("\t - vehicle type: special;\n");
        printf("\t - actual type: %s;\n", garage [i]->specifier.spcl.custom_veh_type);
        break;
      }
      default:
      {
        printf("\t - vehicle type: invalid;\n");
        break;
      }
    }
    switch (garage [i]->eng)
    {
      case 0:
      {
        printf("\t - engine type: gasoline;\n");
        break;
      }
      case 1:
      {
        printf("\t - engine type: diesel;\n");
        break;
      }
      case 2:
      {
        printf("\t - engine type: electric;\n");
        break;
      }
      default:
      {
        printf("\t - engine type: invalid;\n");
        break;
      }
    }
  }
  putchar('\n');
}

int main()
{
  unsigned option;
  unsigned vehicle_qnty = 0;
  Vehicle ** garage = NULL;

  while(1)
  {
    generateMenu();
    scanf("%ud", &option);
    switch (option)
    {
      case 0:
      {
        printf("\nPrint vehicle reference\n");
        printVehicleReference();
        break;
      }
      case 1:
      {
        printf("\nAdd vehicle\n");
        garage = addVehicle(garage, &vehicle_qnty);
        putchar('\n');
        break;
      }
      case 2:
      {
        printf("\nPrint vehicles\n");
        printVehicles(garage, &vehicle_qnty);
        break;
      }
      case 3:
      {
        freeMem(garage, &vehicle_qnty);
        printf("Unreleased memory blocks: %d;\nExiting...\n", malloc_counter);
        exit(EXIT_SUCCESS);
      }
      default:
      {
        printf("Please enter a valid option.\n");
      }
    }
  }
  return 0;
}
