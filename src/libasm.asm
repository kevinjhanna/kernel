GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _keyboard_handler
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti, set_cursor
GLOBAL  _debug

EXTERN  keyboard_handler
EXTERN  scancode_to_ascii

EXTERN  eax_value
EXTERN  ebx_value
EXTERN  ecx_value
EXTERN  edx_value

EXTERN  ebp_value
EXTERN  esp_value

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

_mascaraPIC2:		; Escribe mascara del PIC 2
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
        mov	al,20h			; Envio de EOI generico al PIC
	out	20h,al
	popa
        pop     es
        pop     ds
        iret


; Based on http://www.brokenthorn.com/Resources/OSDev10.html
set_cursor:
  push    ebp
  mov     ebp, esp
  push    ebx
  mov     ebx, [ss: ebp + 8]

  mov al, 0x0f    ; Cursor location low byte index
  mov dx, 0x03D4    ; Write it to the CRT index register
  out dx, al

  mov al, bl      ; The current location is in EBX. BL contains the low byte, BH high byte
  mov dx, 0x03D5    ; Write it to the data register
  out dx, al      ; low byte`

  xor eax, eax

  mov al, 0x0e    ; Cursor location high byte index
  mov dx, 0x03D4    ; Write to the CRT index register
  out dx, al

  mov al, bh      ; the current location is in EBX. BL contains low byte, BH high byte
  mov dx, 0x03D5    ; Write it to the data register
  out dx, al      ; high byte

  pop     ebx
  pop     ebp
  retn






; ***************************************************************************
_keyboard_handler:			; INT 9 Handler (Keyboard)
    push    ds
    push    es      ; Se salvan los registros
    pusha           ; Carga de DS y ES con el valor del selector

    ; Save registers in case we need to show them later
    mov [eax_value], eax
    mov [ebx_value], ebx
    mov [ecx_value], ecx
    mov [ecx_value], edx

    mov [ebp_value], ebp
    mov [esp_value], esp

    ; Read and process scancode
    xor ax, ax      ; Clean ax register
    in al, 60h      ; Load scancode into al register
    push ax         ; Push recently read scancode into stack
    call keyboard_handler

    ; End
    pop ax          ; Pop ascii from stack
    mov	al,20h			; Envio de EOI generico al PIC
    out	20h,al

    popa
    pop     es
    pop     ds
    iret

; ***************************************************************************


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
