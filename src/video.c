#include "../include/kc.h"
#include "../include/file_descriptors.h"
#include "../include/video.h"

char *video = (char *) 0xb8000;
int pos = 0;

ScreenSegment screen_segment_table[10];

void initialize_video() {
  screen_segment_table[DEBUG].start_position = (char *) 0xb8000;
  screen_segment_table[SHELL].start_position = (char *) 0xb8000;
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
