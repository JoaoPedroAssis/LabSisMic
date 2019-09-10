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
			mov		#11011b, R5
			call 	#COD

			jmp 	$
			nop

COD:
			mov 	#0, R6					; Resposta
			mov		#0, R7					; Contador de 1 ums
			mov		#0x8000, R8				; Mascara contadora
			mov		#17, R9
			call	#CONTA_UM				; Conta a quantidades de um em R5, mascara R8, contador R9
COD_LOOP:
			dec		R7
			cmp		#-1, R7
			jeq		COD_FIM
			setc
			rrc		R6
			jmp		COD_LOOP
COD_FIM:
			RET

CONTA_UM:
			dec		R9
			cmp		#0, R9
			jeq		CONTA_UM_FINAL
			BIT		R8, R5
			jc		CONTA_UM_SOMA
			clrc							; Atualiza mascara
			rrc		R8
			jmp		CONTA_UM
CONTA_UM_SOMA:

			inc		R7
			clrc							; Atualiza mascara
			rrc		R8
			jmp		CONTA_UM

CONTA_UM_FINAL:
			RET
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
            
