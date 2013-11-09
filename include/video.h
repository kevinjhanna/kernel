#ifndef _VIDEO_H_
#define _VIDEO_H_

void print_on_screen(char ascii);

void erase_char_on_screen();

int has_char_on_screen();

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Screen Segment descriptor  */
typedef struct {
  char* start_position;
  int offset;
} ScreenSegment;

#endif
