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
main:		mov		#VET1,R5
			mov		#VET2,R6
			mov		#VET3,R7
			call	#MESC
			jmp		$


MESC:		mov		@R5,R8					;tamanho do vetor
			mov		R8,0(R7)				;colocando o tamanho do novo vetor
			add		R8,0(R7)
			incd	R5						;avancando o vetor
			incd	R6						;avancando o vetor
			incd	R7						;avancando o vetor

MESC_Loop:	bit		#1,0(R5)				;verificando se o primeiro valor do vvetor e par
			jc		EhImpar1					;se tiver carry é impar
			mov		@R5,0(R7)				;movendo o valor de R5 para o novo vetor
			incd	R7						;avancando no vetor novo
			mov		@R6,0(R7)				;movendo o valor de R6 para o novo vetor
			incd	R7						;avancando no vetor novo
			jmp		Decisao					;indo para decisao

EhImpar1:	bit		#1,0(R6)				;verificando se o primeiro valor do vvetor e par
			jc		Iguais					;se for impar ir para a parte q ele sao "iguais"
			mov		@R6,0(R7)				;movendo o valor de R6 para o novo vetor
			incd	R7						;avancando no vetor novo
			mov		@R5,0(R7)				;movendo o valor de R5 para o novo vetor
			incd	R7						;avancando no vetor novo
			jmp		Decisao					;indo para decisao

Iguais:		mov		@R5,0(R7)				;movendo o valor de R5 para o novo vetor
			incd	R7						;avancando no vetor novo
			mov		@R6,0(R7)				;movendo o valor de R6 para o novo vetor
			incd	R7						;avancando no vetor novo
			jmp		Decisao					;indo para decisao

Decisao:	dec		R8						;decrementando contador
			jz		MESC_Fim				;se for zero ir para o fim
			incd	R5						;avançando o vetor
			incd	R6						;avançando o vetor
			jmp		MESC_Loop				;voltando para o loop

MESC_Fim:	ret


		.data
VET1:	.word	7,2,-33,4590,-1,-32768,1,2
VET2:	.word	7,3,52,-4596,0,32767,7,2
VET3:	.word	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ;20 zeros

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
            
