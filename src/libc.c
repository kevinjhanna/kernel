#include "../include/kc.h"
#include "../include/kernel.h"
#include "../include/kernel_externals.h"
#include "../include/file_descriptors.h"
#include <stdarg.h> /* lo utilizo en el printf*/

#include "../include/stdio.h"

/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/
unsigned char aBuffer[32];
char * commands[3] ={"openCD", "closeCD", "infoCD"};

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

//https://android.googlesource.com/kernel/lk/+/qcom-dima-8x74-fixes/lib/libc/itoa.c
int
itoa(int num, unsigned char* str, int base)
{
        int sum = num;
        int i = 0;
        int digit;
        unsigned char temp;
        unsigned char* p1;
        unsigned char* p2;

        p1 = str;

        if(sum < 0){
        	str[i++] = '-';
        	p1 = str + 1;
       		sum = -sum;
        }

        do
        {
                digit = sum % base;

                if (digit < 0xA)
                        str[i++] = '0' + digit;
                else
                        str[i++] = 'A' + digit - 0xA;

                sum /= base;

        }while (sum);

        str[i] = '\0';

        p2 = str + i - 1;//position of the last number in the buffer
        
        //this DO WHILE puts in the correct order the given number.
        do
        {
        	temp = *p1;
        	*p1 = *p2;
        	*p2 = temp;
        	p1++;
        	p2--;
        }while(i >= 0 && p2 > p1); 
        return 0;
}

int strcmp (const char * s1, const char * s2)
{
    for(; *s1 == *s2; ++s1, ++s2)
        if(*s1 == '\0')
            return 0;
    return *s1 < *s2 ? -1 : 1;
}

void readCommand(char * cmd)
{
	int aCommand = 0;
	int index = 0;
	int isCommand = -1;
	
	for(; isCommand != 0 && index < sizeof(commands); index++){
		isCommand = strcmp(commands[index], cmd);
	}

	index--;
	switch(index){
		case 0:
			printf("commands: %s", cmd);
			break;
		case 1:
			printf("commands: %s", cmd);
			break;
		case 2:
			printf("commands: %s", cmd);
			break;
		default:
			printf("command is not valid");
			break;
	}

void printfString(char* str){
	do
	{
		putchar(*str);
		str++;
	}while(*str != '\0');
}

char getc(int fd)
{
  // ToDo: Should return int

  char c;
  while(_read(fd, &c, 1) == 0); // Keep on reading from kernel keyboardBuffer
                                // until we have read 1 value.
  return c;
}

char getChar()
{
  getc(STDIN);
}


void 
printf(char * fmt, ...)
{	/*Codigo del libro de C de Kernighan, pag 172*/
	/*mini printf con argumentos variables*/

	va_list ap; /* apunta a cada arg sin nombre en orden */

	char *p, *sval;
	int ival, hval;
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
				itoa(ival, aBuffer, 10);
				printfString(aBuffer);
				break;
			case 'c':
				cval = (char) va_arg(ap, int);
				putchar(cval);
				break;
			case 's':
				for(sval = va_arg(ap, char *); *sval; sval++)
					putchar(*sval);
				break;
			case 'h':
				 hval = va_arg(ap,int);
				 itoa(hval,aBuffer,16);		
				 printfString(aBuffer);
			default:
				putchar(*p);
				break;
		}
	}
	va_end(ap); /*limpia cuando todo esta hecho*/
}

int putchar(int ch)
{	/*http://en.wikibooks.org/wiki/C_Programming/C_Reference/stdio.h/putchar*/
	return putc(ch, SHELL);
}

int putc(int ch, int fd){
  _write(fd, &ch, 1);
	return ch;
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
