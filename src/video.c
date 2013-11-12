#include "../include/kc.h"
#include "../include/file_descriptors.h"
#include "../include/video.h"

char *video = (char *) 0xb8000;
int pos = 0;

ScreenSegment screen_segment_table[10];


/*
 * Text screen video memory is located at 0xb8000
 * The first 10 lines should be the debug screen
 * and the 15 remaining lines for the shell.
 *
 * To calculate the shell screen starting position:
 * Each character on screen takes up two bytes.
 * And each line consits of 80 characters.
 * So, each line should use 160 bytes.
 * We multiply 10d * 160d, and the result we
 * add it to the real starting position.
 * (of course, taking the precautions to use
 * the correct units).
 *
 */
void initialize_video() {
  screen_segment_table[DEBUG].start_position = (char *) 0xb8000;
  screen_segment_table[SHELL].start_position = (char *) 0xb8640;
}


void restart_screen_segment_offset(int fd)
{
  screen_segment_table[fd].offset = 0;
}

/*
 *  Cleans a screen segment and starts offset from 0
 */
void clean_screen_segment(int fd)
{
}

/*
 * Set a white character on given File Descriptor, on given
 * offset.
 * (Only for File Descriptors associated with a screen segment).
 *
 */
void video_set(int fd, int offset, char value){
	screen_segment_table[fd].start_position[offset] = value;
	screen_segment_table[fd].start_position[offset + 1] = WHITE_TXT;
}

void video_write(int fd, char ascii){
  video_set(fd, screen_segment_table[fd].offset, ascii);
  /*
   * We increment two positions since each character
   * on screen takes two bytes.
   */
  screen_segment_table[fd].offset += 2;
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
