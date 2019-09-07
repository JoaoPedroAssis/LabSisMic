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
main:		mov.w	#vetor, R5				; Coloca em R5 o vetor
			call	#subrot
			jmp		$
			nop

subrot:		mov.w	@R5, R8					; Coloca o tam em R8
			clr		R7
			mov.w	#0x00, R6

loop:		inc.w	R5
			cmp.w	R6, 0(R5)
			jhs		maior
			jeq		igual


menor:		jmp		continue

maior:		mov.w	0(R5), R6
			mov.w	#1, R7
			jmp		continue

igual:		inc.w	R7
			jmp		continue

continue:	dec.w	R8
			jnz		loop
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
            
;-------------------------------------------------------------------------------
; Segmento de dados inicializados (0x2400)
;-------------------------------------------------------------------------------
			.data
; Vetor [2 3 5 5 4 2 5 1]

vetor:		.byte 6,0, "JOAQUIMJOSE",0
