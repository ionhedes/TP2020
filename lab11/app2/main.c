#include "database_interface.h"
#include "../dependencies/utils.h"

// Generates nicely formatted menu;
void generateMenu()
{
  printf("-----MENU-----\n\n");
  printf("1. Add person to database.\n");
  printf("2. Delete person from database.\n");
  printf("3. Print database.\n");
  printf("4. Print ordered by wage.\n");
  printf("5. Modify entry.\n");
  printf("6. Quit.\n");
  putchar('\n');
  printf("Enter your option: ");
}

// Driver program for database manipulation;
int main()
{
  int go_condition = 1;
  int return_value;
  unsigned option;
  char * buffer_name = NULL;
  double buffer_wage;

  DEB("DEBUG mode is on. Compile without the -DDEBUG flag to turn it off;\n");

  // If possible, when executing, load the database from the text file first;
  return_value = loadDatabase();

  if (return_value == -1)
  {
    err("Failed to read database from the file.\n");
  }
  else if (return_value == 0)
  {
    printf("There is no database file to be loaded.\n");
    printf("Add persons to the database by selecting option 1, ");
    printf("and then restart the program by selecting option 4, ");
    printf("so a new file is created and the database saved.\n\n");
  }

  // Main loop - user inputs a number, and the actions on the corresponding case
  // are executed;
  while (go_condition)
  {
    generateMenu();
    if (!get_unsigned(&option, stdin))
    {
      freeDatabase();
      err("Failed to read option from the standard input.\n");
    }

    switch (option)
    {
      case 1:
      {
        printf("Adding person to the database...\n");
        printf("\t - enter the person's name: ");
        if ((buffer_name = getString(stdin)) == NULL)
        {
          freeDatabase();
          err("\t\t -failed to read the person's name from the standard input.\n");
        }
        printf("\t - enter the person's wage: ");
        if (!get_double(&buffer_wage, stdin))
        {
          freeDatabase();
          free(buffer_name);
          MALLOC_COUNTER--;
          err("\t\t - failed to read the person's wage from the standard input.\n");
        }
        return_value = addPerson(buffer_name, buffer_wage);
        if (!return_value)
        {
          free(buffer_name);
          MALLOC_COUNTER -= 2;
          freeDatabase();
          OPENED_FILES_COUNTER--;
          err("\t - failed to add person to database from standard input;\n");
        }
        else if (return_value == 2)
        {
          free(buffer_name);
          MALLOC_COUNTER--;
          printf("The person is already listed inside the database;\n");
        }
        putchar('\n');
        break;
      }
      case 2:
      {
        printf("Deleting person from the database...\n");
        printf("\t - enter the person's name: ");
        if ((buffer_name = getString(stdin)) == NULL)
        {
          freeDatabase();
          err("\t\t - failed to read the person's name from the standard input;\n");
        }
        if (!deletePerson(buffer_name))
        {
          printf("\t - the person was not found inside the database;\n");
        }
        else
        {
          printf("\t - the person was deleted;\n");
        }
        free(buffer_name);
        MALLOC_COUNTER--;
        putchar('\n');
        break;
      }
      case 3:
      {
        printf("Printing the database...\n");
        printDatabase();
        putchar('\n');
        break;
      }
      case 4:
      {
        printf("Printing the entries from the database, ordered by their wage...\n");
        printOrderedByWage();
        putchar('\n');
        break;
      }
      case 5:
      {
        char * old_name = NULL;
        char * new_name = NULL;
        double new_wage;

        printf("Modifying database entry...\n");
        printf("Enter the name of the person to be modified: ");
        if ((old_name = getString(stdin)) == NULL)
        {
          freeDatabase();
          err("\t\t - failed to read the person's name from the standard input;\n");
        }

        printf("Enter new data for the person:\n");
        printf("\t - enter the person's name: ");
        if ((new_name = getString(stdin)) == NULL)
        {
          freeDatabase();
          err("\t\t -failed to read the person's new name from the standard input.\n");
        }
        printf("\t - enter the person's wage: ");
        if (!get_double(&new_wage, stdin))
        {
          freeDatabase();
          free(old_name);
          MALLOC_COUNTER--;
          err("\t\t - failed to read the person's new wage from the standard input.\n");
        }

        if (!existsPerson(old_name))
        {
          free(old_name);
          free(new_name);
          MALLOC_COUNTER -= 2;
          printf("The operation cannot be completed because there is no entry bearing the old name in the database.\n");
        }
        else if (existsPerson(new_name))
        {
          free(old_name);
          free(new_name);
          MALLOC_COUNTER -= 2;
          printf("The operation cannot be completed because there is already an entry bearing the new name in the database.\n");
        }
        else
        {
          deletePerson(old_name);
          return_value = addPerson(new_name, new_wage);
          if (!return_value)
          {
            free(old_name);
            free(new_name);
            MALLOC_COUNTER -= 2;
            freeDatabase();
            OPENED_FILES_COUNTER--;
            err("\t - failed to modify database entry;\n");
          }
          free(old_name);
          MALLOC_COUNTER--;
        }
        putchar('\n');
        break;
      }
      case 6:
      {
        go_condition = 0;
        if (database)
        {
          printf("Saving the database to file...\n");
          if (!saveDatabase())
          {
            printf("\t - failed to save the database to the file.\n");
          }
          else
          {
            printf("\t - saved database to file.\n");
          }
          freeDatabase();
        }
        else
        {
          /** For safety reasons, if the database is emptied by the user, the text
              file containing it is deleted from the disk;

              The compilation of the following system command will give a warning,
              but treating it is unnecessary for this program;
          */
          system("test -f \"database.txt\" && rm database.txt");
        }

        printf("Execution ended.\n");
        DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
        DEB("Unclosed files: %d;\n", OPENED_FILES_COUNTER);
        printf("Exiting...\n");
        putchar('\n');
        break;
      }
      default:
      {
        freeDatabase();
        DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
        DEB("Unclosed files: %d;\n", OPENED_FILES_COUNTER);
        err("Default case reached. Error. Freeing memory and aborting.\n");
      }
    }
  }

  return 0;
}
