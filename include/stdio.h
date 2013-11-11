#ifndef _STDIO_H_
#define _STDIO_H_

void printfString(char* str, int stream);
int itoa(int num, unsigned char* str, int base);
char getc(int fd);
char getChar();
int putchar(int ch);
int putc(int ch, int fd);

#endif