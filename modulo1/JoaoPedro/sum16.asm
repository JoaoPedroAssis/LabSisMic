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
main:		mov		#vetor1, R5
            mov		#vetor2, R6
            mov		#vetor3, R7
            mov		@R5+, R8				; Contador
            add		#2, R6					; Incrementando o ponteiro de R6
            add     #2, R7

            call	#SUM16
            jmp		$
            nop

SUM16:		clr     0(R7)
            add		@R5, 0(R7)
		    add		@R6, 0(R7)

		    add		#2, R6
			add		#2,	R5
            add     #2, R7

            dec		R8
            jnz		SUM16
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
; Interrupt Vectors
;-------------------------------------------------------------------------------
			.data
vetor1:		.word   7, 1, 1, 1, 1, 1, 1, 1
vetor2:		.word	7, 1, 1, 1, 1, 1, 1, 1
vetor3:		.word	7
