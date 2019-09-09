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
			mov		#vetor, R5
			call	#EXTREMOS
			jmp		$
			nop

EXTREMOS:
			mov		@R5+, R8				; Inicializa R8 com o tamanho do vetor
			dec		R8						; Subtrai um do tamannho(Estava pegando mais um termo)
			mov		@R5, R6					; Inicializa R6 como o primeiro termo(Maior até o momento)
			mov		@R5+, R7				; Inicializa R7 como o primeito termo(Menos até o momento)


EXTREMOS_loop:
			cmp		R7, 0(R5)				; Compara o elemento atual com o maior
			jge		EXTREMOS_upt_maior		; Se for maior atualiza o maior
			cmp		R6, 0(R5)				; Compara o elemento atual com o menor
 			jl		EXTREMOS_upt_menor		; Se for maior atualiza o menor

EXTREMOS_final_loop:
 			incd	R5						; Atualiza o ponteiro do vetor pra o próximo termo
			dec		R8						; Decrementa contador
			cmp		#0, R8
			jnz		EXTREMOS_loop
			ret								; Caso já tenha percorrido todo o vetor retorna

EXTREMOS_upt_menor:
			mov		@R5, R6
			jmp 	EXTREMOS_final_loop
			nop

EXTREMOS_upt_maior:
			mov		@R5, R7
			jmp		EXTREMOS_final_loop
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
            
;---------------------------------------------------------------------
; Data
;---------------------------------------------------------------------
			.data

; Declarar vetor com 8 elementos [121, 234, 567, -1990, 117, 867, 45, -1980]
vetor:		.word			8, 121, 234, 567, -1990, 117, 867, 45, -1980
