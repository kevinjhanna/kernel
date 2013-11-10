#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/keyboard_driver.h"
#include "../include/kernel_externals.h"
#include "../include/file_descriptors.h"
#include "../include/video.h"
#include "../include/kc.h"
#include "../include/kernel.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

CircularBuffer keyboardBuffer; // variable global

int tickpos=640;

int show_register = false;

void press_ctrl()
{
  show_register = true;
}

void release_ctrl()
{
  show_register = false;
}

void new_ascii(char ascii)
{
    if (ascii == 'r' && show_register)
    {
      char ch = 'x';

      cbWrite(&keyboardBuffer, &ch);

    } else {
      // if(ascii == '\b'){ // si se presiona backspáce ('\b'), se elimina un caracter del buffer del KERNEL
      // cbRead(&keyboardBuffer, &ascii);
      // erase_char_on_screen();
      // return;
      // }
      cbWrite(&keyboardBuffer, &ascii);
    }
}

void key_press(byte scancode)
{
  if (scancode == 0x1d) // TODO: implement with a switch, check other ctrl values.
  {
    press_ctrl();
  }
  else
  {
    char ascii = scancode_to_ascii(scancode);
    new_ascii(ascii);
  }
}

void key_release(byte scancode)
{

  if (scancode == 0x9d) // TODO: implement with a switch, check other ctrl values.
  {
    release_ctrl();
  }
}

/*
 * keyboard_handler function is called with each keyboard interruption.
 * Its parameter is the scancode sent by the keyboard.
 *
 * This function divides key press and key release flow.
 *
 * Implemented by the keyboard driver.
 */

void keyboard_handler(byte scancode) {
  // First we check the most significative bit
  // of the scancode to see if it is a BREAK or MAKE code

  if ((scancode & 0x80) == 0x80) {
    // key release (BREAK CODE)
    key_release(scancode);
  }
  else
  {
    // key press (MAKE CODE)
    key_press(scancode);
  }
}


/*
*Sobre las primitivas: http://www.gnu.org/software/libc/manual/html_node/I_002fO-Primitives.html
*/

int _write(int fd, const void* buffer, int count){
  char* local_buffer = (char *)buffer; // cast void* to char*
  video_write(fd, local_buffer[0]); // to try things out

  // ToDo: implement count > 1

  return count;
}


/* Returns read count */
int _read(int fd, void* buffer, int count){
  if (cbIsEmpty(&keyboardBuffer))
  {
    return 0; // 0 elements found.
  } else {
    cbRead(&keyboardBuffer, buffer); // ToDo: implement iteration for count > 0
    return 1; // should return how many elements it has read.
  }
}

void runShell()
{
  char c;
  while(1)
  {
    c = getChar();
    putchar(c);
  }
}


/**********************************************
kmain()
Punto de entrada de cóo C.
************************************************/
kmain()
{

        int i,num;

/* Borra la pantalla. */

	k_clear_screen();


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

        setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
        setup_IDT_entry (&idt[0x09], 0x08, (dword)&_keyboard_handler, ACS_INT, 0); // hay que poner 0x08 porque es el codesegment

/* Carga de IDTR    */

	idtr.base = 0;
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;

	_lidt (&idtr);

	_Cli();
/* Habilito interrupcion de timer tick*/
  /* habilito teclado */

        _mascaraPIC1(0xFC);
        _mascaraPIC2(0xFF);

	_Sti();


	cbInit(&keyboardBuffer); //inicializo buffer del KERNEL

  initialize_video();
  runShell();

}

