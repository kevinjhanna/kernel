#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/keyboard_driver.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=640;

void int_08() {

    // char *video = (char *) 0xb8000;
    // video[tickpos+=2]='O';

}

void keyboard_handler(char scancode) {
    char *video = (char *) 0xb8000;
    video[tickpos+=2]= scancode_to_ascii(scancode);
}

/**********************************************
kmain()
Punto de entrada de cóo C.
*************************************************/

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

        while(1)
        {
        }

}

