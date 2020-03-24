/////////////////////////////////////////////////
/// myHexdump - hexdump command clone
///           - displays data in the same format as hexdump -C
///           - example:
///             <head of row 1 position offset>  <byte 1>  <byte2>.....<byte 16> | <ascii byte 1>...<ascii byte 16> |
///             ...
///             ...
///             ...
/////////////////////////////////////////////////

#define ROW_LENGTH 16

int printRow(FILE * source)
{
  int i; // < iterator for the bytes on a row
  int eof_index = -1; // < EOF flag
  unsigned char buffer [16]; // < buffer to read row in

  long current_position = ftell(source); // < Current position in file
  if (current_position == -1)
  {
    return 0;
  }
  // Printing the position offset in hex at the beginning of every file
  printf("%08lx", current_position);
  printf("  ");

  // Reading 16 bytes of data and printing them
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
    // feof() must be tested, because if we are in the midst of printing a row
    // when EOF is reached, the rest of the row should be filled with 00
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

  // If the bytes read are printable (their have valid ASCII values),
  // they should be printed, else replaced with a point
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

// Hexdump clone
/**
  - Used initially in the fifth problem of this lab, but I found it useful to have
    it in a header
*/
int myHexdump(FILE * source)
{
  // We read and print blocks of 16 bytes from the file until we reach EOF
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
