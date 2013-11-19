#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#define CB_SIZE 256

/*
** Fuente codigo CircularBuffer
** http://en.wikipedia.org/wiki/Circular_buffer
*/

/* Opaque buffer element type.  This would be defined by the application. */
//typedef struct { int value; } ElemType;

/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
    char   		*elems;  /* vector of elements                   */
} CircularBuffer;

void cbInit(CircularBuffer *cb);

int cbIsFull(CircularBuffer *cb);

int cbIsEmpty(CircularBuffer *cb);

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, char *elem);

/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, char *elem);

extern CircularBuffer cb;

#endif
