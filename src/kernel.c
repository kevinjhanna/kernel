#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/keyboard_driver.h"
#include "../include/kernel_externals.h"
#include "../include/video.h"
#include "../include/kc.h"
#include "../include/kernel.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

CircularBuffer keyboardBuffer; // variable global

int tickpos=640;

void int_08() {
    // char *video = (char *) 0xb8000;
    // video[tickpos+=2]='O';
}

void keyboard_handler(char ascii) {
    if(ascii == '\b'){ // si se presiona backspáce ('\b'), se elimina un caracter del buffer del KERNEL
		cbRead(&keyboardBuffer, &ascii);
		erase_char_on_screen();
		return;
    }
    cbWrite(&keyboardBuffer, &ascii);
    return;
}


/*
*Sobre las primitivas: http://www.gnu.org/software/libc/manual/html_node/I_002fO-Primitives.html
*/

void _write(char c){
  // only temporal
  print_on_screen(c); // to try things out
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

size_t __write(int fd, const void* buffer, size_t count)
{
 //deberiamos poner un switch(fd) case infoRegister: print   case shell: print

  return count;
}

size_t __read(int fd, void* buffer, size_t count)
{

    return count;
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

  runShell();

}

