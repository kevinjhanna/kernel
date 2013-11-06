#include "../include/kc.h"
#include "../include/video.h"

char *video = (char *) 0xb8000;
int pos = 0;


//; deprecated.
void print_on_screen(char ascii){
	video[pos] = ascii;
	video[++pos] = WHITE_TXT;
	pos++;
}

// deprecated.
void erase_char_on_screen(){
	if(has_char_on_screen()){
		pos -= 2;
		video[pos] = ' ';
	}
}

// deprecated.
int has_char_on_screen(){
	return pos > 0;
}
