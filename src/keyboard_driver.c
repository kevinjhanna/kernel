/*
 * Keyboard Driver
 */

#include "../include/keyboard_driver.h"
#include "../include/defs.h"

// Indicates wether ctrl key is pressed or not
int ctrl_pressed = false;

int caplock_pressed = false;


/*
 * Map each scancode to an ascii value.
 *
 */

char asccode[58][2] =
     {
       {   0,0   } ,
       { 0,0 } ,
       { '1','!' } ,
       { '2','@' } ,
       { '3','#' } ,
       { '4','$' } ,
       { '5','%' } ,
       { '6','^' } ,
       { '7','&' } ,
       { '8','*' } ,
       { '9','(' } ,
       { '0',')' } ,
       { '-','_' } ,
       { '=','+' } ,
       { '\b', '\b' } , // backspace
       {   9,9   } ,
       { 'q','Q' } ,
       { 'w','W' } ,
       { 'e','E' } ,
       { 'r','R' } ,
       { 't','T' } ,
       { 'y','Y' } ,
       { 'u','U' } ,
       { 'i','I' } ,
       { 'o','O' } ,
       { 'p','P' } ,
       { '[','{' } ,
       { ']','}' } ,
       { '\n', '\n' } , // enter
       {   0,0   } ,
       { 'a','A' } ,
       { 's','S' } ,
       { 'd','D' } ,
       { 'f','F' } ,
       { 'g','G' } ,
       { 'h','H' } ,
       { 'j','J' } ,
       { 'k','K' } ,
       { 'l','L' } ,
       { ';',':' } ,
       {  39,34  } ,
       { '`','~' } ,
       {   0,0   } ,
       { '\\','|'} ,
       { 'z','Z' } ,
       { 'x','X' } ,
       { 'c','C' } ,
       { 'v','V' } ,
       { 'b','B' } ,
       { 'n','N' } ,
       { 'm','M' } ,
       { ',','<' } ,
       { '.','>' } ,
       { '/','?' } ,
       {   0,0   } , //RShift
       {   0,0   } ,
       {   0,0   } ,
       { ' ',' ' } , //spacebar

   };


char scancode_to_ascii(char scancode) {
  char c = asccode[scancode][0];

  if( ('a' <= c && c <= 'z') && caplock_pressed){
       c = asccode[scancode][1];
  }

  return c;
}

int is_ctrl_pressed(){
  return ctrl_pressed;
}

void chance_caplock_state(){
  caplock_pressed = (caplock_pressed == false);
}

void set_ctrl_pressed(int state){
  ctrl_pressed = state;
}
