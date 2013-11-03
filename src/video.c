#include "../include/kc.h"
#include "../include/video.h"

char *video = (char *) 0xb8000;
int pos = 0;

void print_on_screen(char ascii){
	video[pos] = ascii;
	video[++pos] = WHITE_TXT;
	pos++;
}

void erase_char_on_screen(){
	if(has_char_on_screen()){
		pos -= 2;
		video[pos] = ' ';
	}
}

int has_char_on_screen(){
	return pos > 0;
}