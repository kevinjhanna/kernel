#include "../include/defs.h"
#include "../include/file_descriptors.h"
#include "../include/shell.h"

#define MAX_COMMANDS 8

Command commands[MAX_COMMANDS];

// Initial text to show with each new line.
char* prompt_text = { "user@littlekernel: " };

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

void clear()
{
  clean_screen_segment(SHELL);
  restart_screen_segment_offsets(SHELL);
}

void test_hello_world()
{
  printf("\nHello, world!\n");
}

void test_cd()
{
  int i = 0;
  _openCD();
  while (i < 5000)
  {
    i++;
  }
  _closeCD();

}

void openCD()
{
 _openCD();
}

void closeCD()
{
  _closeCD();
}

void test_zero()
{
  _test_zero();
  printf("\n");
}

void test_numbers()
{
  clear();
  int i, j, t;
  for (t = 0; t < 1000; t++)
  {
    for (i = 0; i < 5; i++)
    {
      for (j = 0; j < 40; j++)
      {
        printf("%d", j);
      }
      printf("\n");
    }
    clear();
  }
}

void help()
{

  printf("\n");
  int i;
  for(i = 0; i < MAX_COMMANDS; i++)
  {
    printf("%s \n", commands[i].name);
  }

}

void initialize_commands()
{
  commands[0].name = "opencd";
  commands[0].function = &openCD;

  commands[1].name = "closecd";
  commands[1].function = &closeCD;

  commands[2].name = "help";
  commands[2].function = &help;

  commands[3].name = "clear";
  commands[3].function = &clear;

  commands[4].name = "test helloworld";
  commands[4].function = &test_hello_world;

  commands[5].name = "test numbers";
  commands[5].function = &test_numbers;

  commands[6].name = "test cd";
  commands[6].function = &test_cd;

  commands[7].name = "test zero";
  commands[7].function = &test_zero;
}

boolean run_command(char * cmd)
{

  int found_command = false;
  int i = 0;

  for(i = 0; i < MAX_COMMANDS && !found_command; i++)
  {

    if (strcmp(commands[i].name, cmd) == 0)
    {
      (*commands[i].function)();
      found_command = true;
    }
  }

  return found_command;
}

void prompt()
{
  printf(prompt_text);
}



void shell_run_command(char * buffer, int* pos)
{
  if (*pos == 0)
  {
    printf("\n");
    return;
  }

  buffer[*pos] = '\0';
  if (!run_command(buffer))
  {
    printf("\nCommand not found \n");
  }

  *pos = 0;
}


void run_shell()
{

  char shell_buffer[1000];
  int shell_buffer_pos = 0;
  char c;

  initialize_commands();
  clean_screen_segment(DEBUG);
  clean_screen_segment(SHELL);
  screen_division();
  prompt();
  while(true)
  {
    c = getChar();

    switch(c)
    {
      case '\n':
        shell_run_command(shell_buffer, &shell_buffer_pos);
        prompt();
        break;
      case '\b':
        if (shell_buffer_pos > 0)
        {
          shell_buffer_pos--;
          video_erase_write(SHELL);
        }
        break;
      default:
        shell_buffer[shell_buffer_pos++] = c;
        putchar(c);
    }
  }
}
