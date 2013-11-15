#ifndef _VIDEO_H_
#define _VIDEO_H_

/* Screen Segment descriptor  */
typedef struct {
  char* start_position;
  int line_offset;
  int char_offset;
  int char_offset_limit;
  int line_offset_limit;
  int bytes_per_char;
} ScreenSegment;

#endif
