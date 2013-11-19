#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/keyboard_driver.h"
#include "../include/kernel_externals.h"
#include "../include/file_descriptors.h"
#include "../include/video.h"
#include "../include/kc.h"
#include "../include/kernel.h"
#include "../include/shell.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

CircularBuffer keyboardBuffer; // variable global

int tickpos=640;

/*
 * Register values stored with each key press.
 */
// at least 32 bits.
unsigned long int eax_value, ebx_value, ecx_value, edx_value;
unsigned long int ebp_value, esp_value;

// at least 16 bits.
unsigned int cs_value, ds_value, ss_value, es_value, fs_value, gs_value;
int idtr_value, flags_value;


/*
 * Function provided by kernel that prints on DEBUG screen
 * the stored register values.
 *
 */

void info_register()
{
  restart_screen_segment_offsets(DEBUG);
  fprintf(DEBUG, "EAX: %h | EBX: %h | ECX: %h | EDX: %h \n", eax_value, ebx_value, ecx_value, edx_value);
  fprintf(DEBUG, "EBP: %h | ESP: %h \n", ebp_value, esp_value);
  fprintf(DEBUG, "CS: %h | DS: %h | ", cs_value, ds_value);
  fprintf(DEBUG, "SS: %h | ES: %h \n", ss_value, es_value);
  fprintf(DEBUG, "GS: %h \n", gs_value);
  fprintf(DEBUG, "FLAGS: %h | IDTR %h \n", flags_value, idtr_value);
}

void key_press(byte scancode)
{
  if (scancode == 0x1d) // TODO: implement with a switch, check other ctrl values.
  {
  	set_ctrl_pressed(true);
  }
  else if(scancode == 0x3a) /* caplock key */
  {
  	chance_caplock_state();
  }
  else if(scancode == 0x2a) /* shift key */
  {
    set_shift_pressed(true);
  }
  else
  {
    char ascii = scancode_to_ascii(scancode);
    if (ascii == 'r' && is_ctrl_pressed())
    {
      info_register(); // Show register info
    } else {
      // write onto keyboard buffer
      cbWrite(&keyboardBuffer, &ascii);
    }
  }
}

void key_release(byte scancode)
{
  switch(scancode){
  	case 0x9d: set_ctrl_pressed(false); break;
  	case 0xaa: set_shift_pressed(false); break;
  }
}

/*
 * keyboard_handler function is called with each keyboard interruption.
 * Its parameter is the scancode sent by the keyboard.
 *
 * This function divides key press and key release flow.
 *
 */

void keyboard_handler(byte scancode) {
  // First we check the most significative bit
  // of the scancode to see if it is a BREAK or MAKE code

  if ((scancode & 0x80) == 0x80) {
    // key release (BREAK CODE)
    key_release(scancode);
  }
  else
  {
    // key press (MAKE CODE)
    key_press(scancode);
  }
}


void _write_new_line(int fd) {
  video_write_new_line(fd);
}

int _write(int fd, const void* buffer, int count){
  char* local_buffer = (char *)buffer;
  video_write(fd, local_buffer[0]); // to try things out

  // ToDo: implement count > 1

  return count;
}

/* Returns read count */
int _read(int fd, void* buffer, int count){
  if (cbIsEmpty(&keyboardBuffer))
  {
    return 0; // 0 elements found.
  } else {
    cbRead(&keyboardBuffer, buffer); // ToDo: implement iteration for count > 0
    return 1; // should return how many elements it has read.
  }
}




/**********************************************
kmain()
Punto de entrada de cóo C.
************************************************/
kmain()
{

        int i,num;

/* Borra la pantalla. */

	k_clear_screen();


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

        setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
        setup_IDT_entry (&idt[0x09], 0x08, (dword)&_keyboard_handler, ACS_INT, 0); // hay que poner 0x08 porque es el codesegment

/* Carga de IDTR    */

	idtr.base = 0;
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;

	_lidt (&idtr);

	_Cli();
/* Habilito interrupcion de timer tick*/
  /* habilito teclado */

        _mascaraPIC1(0xFC);
        _mascaraPIC2(0xFF);

	_Sti();


	cbInit(&keyboardBuffer); //inicializo buffer del KERNEL

  initialize_video();
  run_shell();
}

