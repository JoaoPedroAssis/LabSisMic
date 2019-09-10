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
; Questão Prosta: Mesclar dois vetores com um mesmo tamanho
;	Se um elemento for par e o outro impar, coloca o par
;	Caso contrário coloca o do primeiro vetor
;-------------------------------------------------------------------------------
main:
			mov		#VET1, R5
			mov 	#VET2, R6
			mov		#VET3, R7				; Vetor da resposta
			call	#MESC

			jmp		$
			nop

MESC:
			mov		@R5+, R8				; pula o tamanho no 1 vetor
			add		@R6, R8					; Soma o tamanho dos vetores de entrada

			mov		R8, 0(R7)				; Incializa o tamanho do vetor de resposta
			incd	R7						; Muda o ponteiro de R7 para o começo do vetor

            mov		@R6+, R8				; Contador

MESC_LOOP:
			DEC		R8
			cmp		#-1, R8					; Enquanto não acabar os vetores
			jeq		MESC_FIM				; Se acabar MESC_FIM
			BIT		#0x0001, 0(R5)			; Verifica se é par
			jnc		MESC_DEFAULT			; Caso for par cai no caso padrão já que PAR e PAR/PAR e IMPAR, coloca o primeiro depois o segundo
			jmp		MESC_1_IMPAR			; Caso for impar verifica abaixo

MESC_1_IMPAR:
			BIT		#0x0001, 0(R6)			; Caso o segundo tambem seja impar cai no caso padrão, coloca o primeiro depois o segundo
			jc		MESC_DEFAULT			; Caso não seja o 1 é par e o segundo é impar, então:
			mov		@R6+, 0(R7)				; Adiciona primeiro o segundo vetor
			incd	R7
			mov		@R5+, 0(R7)				; Posteriormente o segundo vetor
			incd	R7
			jmp 	MESC_LOOP				; Volta para o loop

MESC_DEFAULT:
			mov		@R5+, 0(R7)				; Caso default, prmeiro adiciona o vetor1 depois o vetor2
			incd	R7
			mov		@R6+, 0(R7)
			incd	R7
			jmp 	MESC_LOOP				; Volta para o loop

MESC_FIM:
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
            
            .data
VET1:		.word	7,2,-33,4590,-1,-32768,1,2
VET2:		.word	7,3,52,-4596,0,32767,7,2
VET3:		.word	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ;20 zeros
