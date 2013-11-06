#include "../include/kc.h"
#include "../include/kernel.h"
#include "../include/kernel_externals.h"
#include <stdarg.h> /* lo utilizo en el printf*/

/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() 
{
	/*char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]='7';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};*/
}

char getChar()
{
  // ToDo: Should call getc(..)
  // ToDo: Should return int

  char c[1];                  // We create a local buffer with only one element.
  while(_read(1, c, 1) == 0); // Keep on reading from kernel keyboardBuffer.
  return c[0];                // Return first and only element of local buffer.
}

printf(char * fmt, ...)
{	/*Codigo del libro de C de Kernighan, pag 172*/
	/*mini printf con argumentos variables*/

	va_list ap; /* apunta a cada arg sin nombre en orden */

	char *p, *sval;
	int ival;
	char cval;

	va_start(ap,fmt); /*hace que ap apunte al 1er. arg sin nombre*/
	for(p = fmt; *p; p++){
		if( *p != '%'){
			putchar(*p);
			continue;
		}
		switch(*++p){
			case 'd':
				ival = va_arg(ap,int);
				/*printINT();*/
				break;
			case 'c':
				cval = (char) va_arg(ap, int);
				putchar(cval);
				break;
			case 's':
				for(sval = va_arg(ap, char *); *sval; sval++)
					putchar(*sval);
				break;
			default:
				putchar(*p);
				break;
		}
	}
	va_end(ap); /*limpia cuando todo esta hecho*/
}

int putchar(int ch)
{	/*http://en.wikibooks.org/wiki/C_Programming/C_Reference/stdio.h/putchar*/
	return putc(ch, 1); //file descriptor del SHELL
}

int putc(int ch, int fd){

	// __write(fd, &ch, 1);	/*escribe en pantalla*/
	_write(ch); //only temporal

	return ch; /*el caracter escribo es retornado*/
}


/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}
