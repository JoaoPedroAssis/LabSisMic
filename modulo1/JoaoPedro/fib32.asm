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
; Pares ->	R11 (LSW) e R10 (MSW)
;			R6 	(LSW) e R5	(MSW)
;			R8	(LSW) e R7	(MSW)
;			R13	(LSW) e R12	(MSW)

main:		clr		R10
			clr		R11
			clr 	R14
			call	#FIB32
			jmp		$
			nop

FIB32:		mov		#0, R5					; Numero atual fib
			mov		#1, R6

			mov		#0, R7					; Numero anterior fib
			mov		#1,	R8

loop:		inc		R14
			mov		R5, R12					; Guarda numero fib atual
			mov		R6, R13

			add		R8, R6					; Soma o LSW
			jc		carry					; Soma o carry se tiver
retCarry:	add		R7, R5					; Soma o MSW

			mov		R13, R8
			mov		R12, R7

			jnc		loop
			mov		R8, R11
			mov		R7, R10
			ret

carry:
			adc		R5
			jmp		retCarry
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
            
