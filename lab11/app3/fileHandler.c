#include "database_interface.h"
#include "../dependencies/utils.h"

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
    fprintf(file, "%s;%c;%f\n", current->name, current->gender, current->wage);
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
  int semicol1_pos;
  int semicol2_pos;
  char * buffer_string = NULL;
  char buffer_gender;
  double buffer_wage;

  DEB("Loading database from file...\n");
  if (!file)
  {
    DEB("File failed to open. This may be because the file does not exist, or because another error happened.\n");
    return 0;
  }
  OPENED_FILES_COUNTER++;
  freeDatabase();

  while(!ferror(file))
  {
    /** Every entry from the database file is read as a whole line, and then parsed;
        - a line without two semicolons is considered invalid and is skipped;
        - before the first semicolon is the name of the person;
        - between the first and the second semicolon is the gender of the person;
        - after the second semicolon, and before \0 is the wage of the person;
    */
    if ((buffer_string = getString(file)) == NULL)
    {
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


    semicol1_pos = strcspn(buffer_string, ";");
    semicol2_pos = semicol1_pos + 1 + strcspn(buffer_string + semicol1_pos + 1, ";");
    if (semicol1_pos == strlen(buffer_string) || semicol2_pos == strlen(buffer_string))
    {
      DEB("\t\t - the line is invalid (no semicolon), ignoring it...\n");
      free(buffer_string);
      MALLOC_COUNTER--;
      continue;
    }
    buffer_wage = atof(buffer_string + semicol2_pos + 1);
    buffer_gender = *(buffer_string + semicol1_pos + 1);
    buffer_string = (char *)realloc(buffer_string, semicol1_pos + 1);
    buffer_string [semicol1_pos] = '\0';

    DEB("\t\t - extracted name: %s;\n", buffer_string);
    DEB("\t\t - extracted wage: %lf;\n", buffer_wage);

    if (!addPerson(buffer_string, buffer_gender, buffer_wage))
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
