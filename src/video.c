#include "../include/kc.h"
#include "../include/file_descriptors.h"
#include "../include/video.h"

char *video = (char *) 0xb8000;
int pos = 0;

ScreenSegment screen_segment_table[10];

// ***************************************************************************

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

void restart_screen_segment_offsets(int fd)
{
  screen_segment_table[fd].char_offset = 0;
  screen_segment_table[fd].line_offset = 0;
}

void initialize_screen_segment(int fd, int start_position, int end_position)
{
  screen_segment_table[fd].start_position = (char *) start_position;
  screen_segment_table[fd].char_offset_limit = 80;
  screen_segment_table[fd].bytes_per_char = 2;
  restart_screen_segment_offsets(fd);
}

void initialize_video() {
  initialize_screen_segment(DEBUG, 0xb8000, 0xfffff);
  initialize_screen_segment(SHELL, 0xb8640, 0xfffff);
}

// ***************************************************************************


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
void video_set(int fd, int char_offset, int line_offset, char value){
  ScreenSegment ss = screen_segment_table[fd];

  int position = line_offset * ss.char_offset_limit + char_offset;
  position *= ss.bytes_per_char;

	ss.start_position[position] = value;
	ss.start_position[position + 1] = WHITE_TXT;
}

void video_write(int fd, char ascii){
  ScreenSegment* ss = &screen_segment_table[fd];
  video_set(fd, ss->char_offset, ss->line_offset, ascii);
  /*
   * We increment one positions since we only
   * count logic characters, not bytes on screen.
   */
  ss->char_offset++;

  if (ss->char_offset == ss->char_offset_limit)
  {
    ss->char_offset = 0;
    ss->line_offset++;
  }
}

void video_write_new_line(int fd)
{
  screen_segment_table[fd].line_offset++;
  screen_segment_table[fd].char_offset = 0;
  // TODO Check if we are off limits!!
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
