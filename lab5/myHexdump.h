#define ROW_LENGTH 16

int printRow(FILE * source)
{
  int i;
  int eof_index = -1;
  char buffer [16];

  // Offset
  long current_position = ftell(source);
  if (current_position == -1)
  {
    return 0;
  }
  printf("%08lx", current_position);
  printf("  ");

  // Hex data
  for (i = 0; i < ROW_LENGTH; i++)
  {
    if (i == 8)
    {
      putchar(' ');
    }
    if (fread(&buffer [i], 1, 1, source) != 1 && ferror(source))
    {
      fprintf(stderr, "\nFailed to read from file.\n");
      return 0;
    }
    if (feof(source))
    {
      if (eof_index == -1)
      {
        eof_index = i;
      }
      printf("%02x ", 0);
    }
    else
    {
      printf("%02x ", buffer [i]);
    }
  }
  printf(" |");

  // ASCII data
  for (i = 0; i < ROW_LENGTH && i != eof_index; i++)
  {
    if (buffer [i] >= 32 && buffer [i] <= 255)
    {
      putchar(buffer [i]);
    }
    else
    {
      putchar('.');
    }
  }
  putchar('|');

  return 1;
}

int myHexdump(FILE * source)
{
  while(!feof(source) && !ferror(source))
  {
    if (!printRow(source))
    {
      return 0;
    }
    putchar('\n');
  }
  return 1;
}
