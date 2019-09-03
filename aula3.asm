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
MAIN:		  mov		#vetor, R5
          clr		R6
          clr		R7
          clr		R10
          mov.b	@R5, R10
          call	#M2M4
          jmp		$
          nop

M2M4:		  dec		R10
          inc		R5
          mov.b	@R5, R9
          mov.b	#2, R8
          call	#ISMULT
          add		R9, R6

          mov.b	@R5, R9
          mov.b	#4, R8
          call	#ISMULT
          add		R9, R7

          cmp		#0, R10
          jge		M2M4
          ret

                                            
ISMULT:		  sub		R8, R9
            jlo		OUTROROT
            jmp		ISMULT
            nop
SAIDA:		  ret

OUTROROT:	  clr		R11
            cmp		R9, R11
            jlo		SUM
            clr		R9
            jmp 	SAIDA
            nop

SUM:		    mov		#1, R9
			      jmp		SAIDA
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
; Data
;-------------------------------------------------------------------------------
			.data
vetor:		.byte	9,"JOAOPEDRO"
