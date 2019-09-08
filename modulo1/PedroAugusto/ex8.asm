;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
; Além de Mostrar em R10 mostra na memória
main:
			mov		#fb_result,	R5			; Inicializa vetor do resultado
			mov		#1, R6					; Inicializa semente 1 em R6
			mov		#1, R7					; Inicializa semente 2 em R7

			call	#FIB16
			jmp		$
			nop
FIB16:
			mov		R6,	0(R5)				; Inializa vetor com as sementes
			incd	R5
			mov		R7,	0(R5)
			incd	R5

FIB16_loop:
			clr 	R9
			add		R6, R9
			add		R7, R9					; R9 = R7 + R6
			cmp		#0,	R9
			jge		FIB16_loop_add			; Se R9 for negativ deu overflow
			mov		R6,	R10
			mov		#-1111, 0(R5)			; Mostrar que parou
			RET
                                            
FIB16_loop_add:
			mov		R9, 0(R5)				; Adiciona termo no vetor resultado
			mov		R6, R7					; Atualiza 2 ultimos termos
			mov 	R9, R6
			incd	R5						; Atualiza ponteiro do vetor resultado
			jmp		FIB16_loop
			nop
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
     		.data

fb_result:	.word	1
