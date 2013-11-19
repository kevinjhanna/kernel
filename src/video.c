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

void initialize_screen_segment(int fd, int start_position, int lines_limit)
{
  screen_segment_table[fd].start_position = (char *) start_position;
  screen_segment_table[fd].line_offset_limit = lines_limit;
  screen_segment_table[fd].char_offset_limit = 80;
  screen_segment_table[fd].bytes_per_char = 2;
  restart_screen_segment_offsets(fd);
}

void initialize_video() {
  initialize_screen_segment(DEBUG, 0xb8000, 10);
  initialize_screen_segment(SHELL, 0xb8640, 15);
}

// ***************************************************************************

/*
 * Set a white character on given File Descriptor, on given
 * offset.
 * (Only for File Descriptors associated with a screen segment).
 *
 */
void video_set(int fd, int char_offset, int line_offset, char value){
  ScreenSegment ss = screen_segment_table[fd];

  int logic_position = line_offset * ss.char_offset_limit + char_offset;
  int byte_position = logic_position * ss.bytes_per_char;

	ss.start_position[byte_position] = value;
	ss.start_position[byte_position + 1] = WHITE_TXT;

}

void video_move_type_cursor(int fd)
{
  ScreenSegment* ss = &screen_segment_table[fd];
  // TODO: move this magic number to a constant
  set_cursor((10 + ss->line_offset) * ss->char_offset_limit + ss->char_offset);
}

void _video_new_line(int fd)
{
  screen_segment_table[fd].line_offset++;
  screen_segment_table[fd].char_offset = 0;
  // TODO Check if we are off limits!!
}

void video_write_new_line(int fd)
{
  _video_new_line(fd);
  video_move_type_cursor(fd);
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
    _video_new_line(fd);
  }

  video_move_type_cursor(fd);
}

/*
 * Erase previous written position.
 * Does not support multi lines erasing.
 */
void video_erase_write(int fd)
{
  ScreenSegment* ss = &screen_segment_table[fd];
  if (ss->char_offset > 0)
  {
    ss->char_offset--;
    video_set(fd, ss->char_offset, ss->line_offset, ' ');
    video_move_type_cursor(fd);
  }
}


/*
 *  Cleans a screen segment and starts offset from 0
 */
void clean_screen_segment(int fd)
{
  ScreenSegment ss = screen_segment_table[fd];

  int line;
  int char_pos;
  for (line = 0; line < ss.line_offset_limit; line++)
  {
    for (char_pos = 0; char_pos < ss.char_offset_limit; char_pos++)
    {
      video_set(fd, char_pos, line, ' ');
    }
  }
}

void screen_division()
{
  ScreenSegment screen_segment = screen_segment_table[DEBUG];
  int offset;
  for(offset = 0; offset <  screen_segment.char_offset_limit; offset++)
  {
    video_set(DEBUG, offset, screen_segment.line_offset_limit - 1, '-');
  }
}
