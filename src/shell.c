#include "../include/defs.h"
#include "../include/shell.h"

#define MAX_COMMANDS 3

Command commands[MAX_COMMANDS];

int strcmp (const char * s1, const char * s2)
{
  for(; *s1 == *s2; ++s1, ++s2)
  {
    if(*s1 == '\0')
    {
      return 0;
    }
  }
  return *s1 < *s2 ? -1 : 1;
}

void test_hello_world()
{
  printf("Hello, world!");
}

void openCD()
{
 _openCD();
}

void closeCD()
{
  _closeCD();
}

void infoCD()
{
  printf("Hello, world!");
}


void initialize_commands()
{
  commands[0].name = "opencd";
  commands[0].function = &openCD;

  commands[1].name = "closecd";
  commands[1].function = &closeCD;

  commands[2].name = "infocd";
  commands[2].function = &infoCD;
}

boolean run_command(char * cmd)
{

  int found_command = false;
  int i = 0;

  for(i = 0; i < 3 && !found_command; i++)
  {

    if (strcmp(commands[i].name, cmd) == 0)
    {
      (*commands[i].function)();
      found_command = true;
    }
  }

  return found_command;
}

void shell_run_command(char * buffer, int* pos)
{

  if (*pos > 0)
  {
    buffer[*pos] = '\0';
    if (!run_command(buffer))
    {
      printf("Command not found \n");
    }

    *pos = 0;
  }
}

void run_shell()
{

  char shell_buffer[10];
  int shell_buffer_pos = 0;
  char c;

  initialize_commands();

  while(true)
  {
    c = getChar();
    putchar(c);

    if (c == '\n')
    {
      shell_run_command(shell_buffer, &shell_buffer_pos);
    }
    else
    {
      shell_buffer[shell_buffer_pos++] = c;
    }
  }
}
