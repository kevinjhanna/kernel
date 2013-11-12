/***************************************************
  Shell.h

****************************************************/

#ifndef _shell_
#define _shell_

void run_shell();

 typedef struct {
   char* name;
   void (*function)();
 } Command;

#endif

