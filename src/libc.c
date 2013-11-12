#include "../include/kc.h"
#include "../include/kernel.h"
#include "../include/kernel_externals.h"
#include "../include/file_descriptors.h"
#include <stdarg.h> /* lo utilizo en el printf*/

#include "../include/stdio.h"
#include "../include/string.h"

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
int itoa(int sum, unsigned char* str, int base)
{

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

  } while (sum);

  str[i] = '\0';

  p2 = str + i - 1; // Position of the last number in the buffer

  // Now we put in the correct order the given number.
  do
  {
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
    p1++;
    p2--;
  } while(i >= 0 && p2 > p1);

  return 0;
}

// TODO this is just copypaste from atoi but with unsigned int
// variables. Should try to refactor this to be DRY

unsigned int strtoul(unsigned int sum, unsigned char* str, int base)
{

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

  } while (sum);

  str[i] = '\0';

  p2 = str + i - 1; // Position of the last number in the buffer

  // Now we put in the correct order the given number.
  do
  {
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
    p1++;
    p2--;
  } while(i >= 0 && p2 > p1);

  return 0;
}


void printfString(char* str, int stream){
	do
	{
		putc(*str, stream);
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


int strcmp (const char * s1, const char * s2)
{
    for(; *s1 == *s2; ++s1, ++s2)
        if(*s1 == '\0')
            return 0;
    return *s1 < *s2 ? -1 : 1;
}

int fprintf(int stream, const char *fmt, ...){

	va_list ap; /* apunta a cada arg sin nombre en orden */
	va_start(ap,fmt); /*hace que ap apunte al 1er. arg sin nombre*/

	vfprintf(stream, &fmt, ap);

	va_end(ap); /*limpia cuando todo esta hecho*/

	return 0;
}


/*
 * Basic vfprintf with variable arguments
 * function based on Kernighan's C book, page 172.
 *
 * Format rules:
 *
 * %s for strings.
 * %d for decimals.
 * %c for chars.
 * %h for hexadecimal (unsigned).
 *
 */
int vfprintf(int stream, char *fmt,va_list ap)
{

	char *p, *sval;
	int ival, hval;
	char cval;

		for(p = fmt; *p; p++){
		if( *p != '%'){
			putc(*p,stream);
			continue;
		}
		switch(*++p){
			case 'd':
				ival = va_arg(ap,int);
				itoa(ival, aBuffer, 10);
				printfString(aBuffer, stream);
				break;
			case 'c':
				cval = (char) va_arg(ap, int);
				putc(cval, stream);
				break;
			case 's':
				for(sval = va_arg(ap, char *); *sval; sval++)
					putc(*sval, stream);
				break;
			case 'h':
				 hval = va_arg(ap, unsigned int);
				 strtoul(hval, aBuffer, 16);
				 printfString(aBuffer, stream);
			default:
				putc(*p, stream);
				break;
		}
	}

	return 0;
}

void printf(char * fmt, ...)
{

	va_list ap; /* apunta a cada arg sin nombre en orden */
	va_start(ap,fmt); /*hace que ap apunte al 1er. arg sin nombre*/

	vfprintf(SHELL, fmt, ap);

	va_end(ap); /*limpia cuando todo esta hecho*/
}

int putchar(int ch)
{	/*http://en.wikibooks.org/wiki/C_Programming/C_Reference/stdio.h/putchar*/
	return putc(ch, SHELL);
}

int putc(int ch, int fd){
  if (ch == '\n')
  {
    _write_new_line(fd);
    return 0;
  }
  else
  {
    _write(fd, &ch, 1);
    return ch;
  }
}


int isNumber(char ascii)
{
	return ascii >= '0' && ascii <= '9';
}

int isHexa(char ascii)
{
	return (ascii>='a' && ascii<='d') || (ascii>='A' && ascii<='D') || isNumber(ascii);
}

int isSpace(char ch)
{
	return ch == ' ' || ch == '\n' || ch == '\t';
}

void scanfDecimal(int *pint)
{
  char ch;
  int decimal;

  ch = getChar();
  putchar(ch);
  while(!isSpace(ch) && isNumber(ch)){
    decimal = ch-'0';
    (*pint)= (*pint)*10+decimal;
    ch = getChar();
    putchar(ch);
  }
}

int hexToDecimal(char ch)
{
	int rta;
	switch(ch){
			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': 
				rta=(ch-'a'+10);
				break;
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': 
				rta=(ch-'A'+10);
				break;
			default: 
				rta=(ch-'0');
				break;
	}

	return rta;
}

void scanfhexa(int *pint)
{
	int hex;
	char ch;

	ch = getChar();
	putchar(ch);
	while(!isSpace(ch) && isHexa(ch))
	{
		hex = hexToDecimal(ch);
		*pint = (*pint)*16 + hex;
		ch = getChar();
		putchar(ch);
	}
}

void scanfchar(char* pchar)
{
  char ch;
  int i=1;
  
  ch=getChar();
  putchar(ch);
  
  while(!isSpace(ch))
  {
    pchar[i]=ch;
    i++;
    ch=getChar();	
    putchar(ch);
  }

  pchar[i]='\0';
}

int scanf(const char *fmt, ...)
{
		va_list ap;
		int i=0;
		int cant = 0;
		char ch;
		int* pint;
		char* pchar;

		va_start(ap,fmt);

		while(fmt[i] != 0)
		{
 			ch = getChar();
			putchar(ch);
	
			if(fmt[i] == '%'){
				i++;
				switch(fmt[i])
				 {
				    case 'd':  
						pint=va_arg(ap, int *);
						if(isNumber(ch)){
							(*pint)=ch-'0';
							scanfDecimal(pint);
						}					
						i++;
						cant++;
						break;
				    case 's':
						pchar=va_arg(ap,char *);
						scanfchar(pchar);
						i++;
						cant++;
						break;
					case 'x':
						pint=va_arg(ap, int *);
						if(isHexa(ch)){
							*pint = hexToDecimal(ch);
							scanfhexa(pint);
						}
						i++;
						cant++;
						break;					 						
				  }	
			}
		}
		va_end(ap);
		return i;
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
