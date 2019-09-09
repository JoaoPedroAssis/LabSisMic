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
; Pares de registradores R6(MSW) R5 (LSW)
; Pares de registradores R8(MSW) R7 (LSW)
; Pares de registradores SOMA R10(MSW) R9 (LSW)
main:
			mov		#1, R5					; Inicializa semente 1 em R6
			mov		#1, R7					; Inicializa semente 2 em R7

			call	#FIB32
			jmp		$
			nop
FIB32:
			clr		R8
			clr		R6

FIB32_loop:
			clr		R10
			clr 	R9
			add		R5, R9					; Somando os menos significativos
			add		R7, R9
			addc	#0, R10					; Somando o carry da operação passada
			add		R6, R10					; Somando mais significativos
			add		R8, R10
			cmp		#0,	R10					; Se der overflow no mais significativo
			jge		FIB32_loop_change
			mov		R7, R10					; Monta a resposta
			mov		R8, R11
			ret

FIB32_loop_change:
			mov		R7, R5					; Atualiza o valor mais antigo
			mov		R8, R6
			mov		R9, R7					; Atualiza o valor mais novo
			mov		R10, R8
			jmp		FIB32_loop
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

