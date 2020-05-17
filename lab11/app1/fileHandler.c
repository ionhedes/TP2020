#include "database_interface.h"
#include "utils.h"

int saveDatabase()
{
  Person * current = NULL;
  FILE * file = fopen("database.txt", "wb");

  DEB("Saving database to file...\n");
  if (!file)
  {
    DEB("Failed to create/open file;\n");
    freeDatabase();
    return 0;
  }
  OPENED_FILES_COUNTER++;
  DEB("Iterating database to store it on disk...\n");
  for (current = database; current; current = current->next)
  {
    DEB("\t - person name: %s;\n", current->name);
    fprintf(file, "%s;%f\n", current->name, current->wage);
  }
  fclose(file);
  OPENED_FILES_COUNTER--;
  DEB("Files still open: %d;\n", OPENED_FILES_COUNTER);
  DEB("Saving database complete;\n\n");
  return 1;
}

int loadDatabase()
{
  FILE * file = fopen("database.txt", "rb");
  int semicol_pos;
  char * buffer_string = NULL;
  double buffer_wage;

  DEB("Loading database from file...\n");
  if (!file)
  {
    DEB("File failed to open. This may be because the file does not exist,");
    DEB("or because another error happened.\n");
    return 0;
  }
  OPENED_FILES_COUNTER++;
  freeDatabase();

  while(!ferror(file))
  {
    if ((buffer_string = getString(file)) == NULL)
    {
      /** Every entry from the database file is read as a whole line, and then parsed;
          - a line without a semicolon is considered invalid and is skipped;
          - before the semicolon is the name of the person;
          - after the semicolon, and before \0 is the wage of the person;
      */
      if (!feof(file))
      {
        DEB("\t - failed to read a name from the file;\n");
        freeDatabase();
        fclose(file);
        OPENED_FILES_COUNTER--;
        return -1;
      }
      else
      {
        DEB("\t - EOF of database file reached;\n");
        break;
      }
    }
    DEB("\t - new line from file: %s;\n", buffer_string);


    semicol_pos = strcspn(buffer_string, ";");
    if (semicol_pos == strlen(buffer_string))
    {
      DEB("\t\t - the line is invalid (no semicolon), ignoring it...\n");
      free(buffer_string);
      MALLOC_COUNTER--;
      continue;
    }
    buffer_wage = atof(buffer_string + semicol_pos + 1);
    buffer_string = (char *)realloc(buffer_string, semicol_pos + 1);
    buffer_string [semicol_pos] = '\0';

    DEB("\t\t - extracted name: %s;\n", buffer_string);
    DEB("\t\t - extracted wage: %lf;\n", buffer_wage);

    if (!addPerson(buffer_string, buffer_wage))
    {
      DEB("\t\t - failed to add person to database from file;\n");
      free(buffer_string);
      MALLOC_COUNTER--;
      freeDatabase();
      fclose(file);
      OPENED_FILES_COUNTER--;
      return -1;
    }
  }
  if (ferror(file))
  {
    DEB("Input file error;\n");
    freeDatabase(database);
    fclose(file);
    OPENED_FILES_COUNTER--;
    return -1;
  }

  fclose(file);
  OPENED_FILES_COUNTER--;
  DEB("Files still open: %d;\n", OPENED_FILES_COUNTER);
  DEB("Importing database complete;\n\n");
  return 1;
}
