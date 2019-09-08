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
main:
			mov		#Vetor1, R5
			mov		#Vetor2, R6
			mov		#ResultSoma, R7
			call	#SUM16

			jmp		$
			nop

SUM16:
			mov		@R5+, R8				; R8 contador do tamanho
			incd	R6						; Alinha o ponteiro de R6 para o começo do vetor
			mov		R8, 0(R7)				; Coloca o tamanho do vetor em R7
			incd	R7						; Alinha o ponteiro do resultado para o começo do vetor

SUM16_loop:
			mov		@R5+, R9				; Guarda os valores da posição do vetor para somar
			mov		@R6+, R10
			add		R9, R10
			mov		R10, 0(R7)				; Guarda o valor da soma no resultado
			incd	R7						; Incrementa a posição do vetor resultado
			dec		R8						; Decrementa o contador
			cmp		#0, R8
			jne		SUM16_loop
			ret
                                            

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
            
;---------------------------------------------------------------------
; Data
;---------------------------------------------------------------------
			.data

Vetor1:		.word		7, 65000, 50054, 26472, 53000, 60606, 814, 41121
Vetor2:		.word		7, 226, 3400, 26472, 470, 1020, 44444, 12345
ResultSoma:	.word		7
