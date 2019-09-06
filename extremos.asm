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
main:       mov 	#vetor, R5			          ; Putting vector pointer in R5
            mov		@R5+, R8				          ; Putting vector size in R8
            mov		@R5+, R6				          ; Initializing smaller as the first
            mov		@R5+, R7				          ; Initializing bigger as the second
            sub		#2, R8 					          ; Correcting counter
            call	#EXTREMOS
            jmp		$
            nop

EXTREMOS:	  cmp		R7, 0(R5)				          ; 0(R5) - R7
			      jge		swapHigh				          ; Swap if 0(R5) > R7
retHigh:	  cmp 	R6, 0(R5)				          ; 0(R5) - R6
			      jl		swapLow					          ; Swap if 0(R5) < R6
retLow:		  add		#2, R5
			      dec		R8
			      jnz		EXTREMOS
			      ret

swapHigh:	  mov		@R5, R7
			      jmp		retHigh
			      nop

swapLow:	  mov 	@R5, R6
			      jmp		retLow
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
            
;-------------------------------------------------------------------------------
; Data Initialization
; 17/0146367 2001 -> 170, 146, 367, -2001
; 19/0994361 1998 -> 190, 994, 361, -1998
;-------------------------------------------------------------------------------
			.data
vetor:		.word		8, 170, 146, 367, -2001, 190, 994, 361, -1998
