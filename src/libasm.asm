GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _keyboard_handler
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug

EXTERN  int_08
EXTERN  keyboard_handler
EXTERN  scancode_to_ascii

SECTION .text


_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR
	rol	ebx,16
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_int_08_hand:				; Handler de INT 8 ( Timer tick)
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           ; Carga de DS y ES con el valor del selector
        mov     ax, 10h			; a utilizar.
        mov     ds, ax
        mov     es, ax
        call    int_08
        mov	al,20h			; Envio de EOI generico al PIC
	out	20h,al
	popa
        pop     es
        pop     ds
        iret


_keyboard_handler:			; INT 9 Handler (Keyboard)
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           ; Carga de DS y ES con el valor del selector

        in ax, 60h      ; Load scancode into ax register
        mov dx, ax      ; We will check the most significative bit
        and dx, 80h     ; of the scancode to see if it is a BREAK or MAKE code
        cmp dx, 80h
        je _keyboard_handler_end ; Exit if scancode means a key press release.
                        ; Otherwise...
        push ax         ; Push recently read scancode into stack
        call scancode_to_ascii ; Get ascii value
        pop dx          ; Pop parameter
        push ax         ; ascii value is now storead in EAX register.
        call keyboard_handler
        pop ax          ; Pop ascii from stack
  _keyboard_handler_end:
        mov	al,20h			; Envio de EOI generico al PIC
        out	20h,al
        popa
        pop     es
        pop     ds
        iret


; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0
;


_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn
