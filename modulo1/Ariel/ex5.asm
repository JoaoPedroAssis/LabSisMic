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
main:		mov		#vetor1,R5				;inicializar R5 com o endereço do vetor
			mov		#vetor2,R6
			call	#SUM16					;chama a subrotina
			jmp		$
			nop


SUM16:		push	R10						;salvando contexto
			push 	R11						;salvando contexto
			push	R12						;salvando contexto
			clr		R11						;zerando R11
			mov		@R5+,R10				;Colocando o tamnho do vetor em R10
			mov		R10,R12					;guardando o valor para o final
			incd	R6						;incrementando pocisao do R6
			mov		R6,R7					;ajeitando posiçao do vetor de saida
			add		R10,R7					;ajeitando posiçao do vetor de saida
			add		R10,R7					;ajeitando posiçao do vetor de saida
			mov		R10,0(R7)				;colocando o tamanho do novo vetor
			incd	R7						;avançando R7

SUM16_Loop:	add		@R5,R11					;Colocando valor em R11
			add		@R6,R11					;somando o valor dos dois vetores e guardando no R11
			mov		R11,0(R7)				;colocando resultado no vetor resultado
			jmp		Decisao					;indo para decisao

Decisao:	dec		R10						;decrementando contador de loops
			jz		SUM16_Fim				;condicao de termino do loop
			clr		R11						;zerando R11
			incd	R5						;avancando R5
			incd	R6						;avancando R6
			incd	R7						;avancando R7
			jmp		SUM16_Loop				;voltando ao loop

SUM16_Fim:
			sub		R12,R7					;voltando vetor a pocisao inicial
			sub		R12,R7					;voltando vetor a pocisao inicial
			pop 	R12						;retornando contexto
			pop 	R11						;retornando contexto
			pop 	R10						;retornando contexto






			.data
vetor1:		.word	7, 1, 2, 3, 4, 5, 6, 7 		;declaraçao do vetor e seu conteudo
vetor2:		.word	7, 7, 6, 5, 4, 3, 2, 1
                                            

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
            
