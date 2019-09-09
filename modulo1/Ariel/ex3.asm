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
main:		mov		#vetor,R5				;inicializar R5 com o endereço do vetor
			call	#M2M4					;chama a subrotina
			jmp		$
			nop


M2M4:		push	R10						;salvando contexto	R10=tamanho do vetor
			push	R11						;salvando contexto	R11=Bit de comparacao para o bit
			mov.b	@R5+,R10				;salvando o tamanho do vetor no R10
			mov		#1,R11					;inicializando R11
			mov		#0,R6					;inicializando R6 = multiplo de 2
			mov 	#0,R7					;inicializando R7 = multiplo de 4

M2M4_Loop:	bit.b	#1,0(R5)				;fazendo o bit test com 00000001
			jnc		M2T					;se nao tiver carry é multiplo de 2
			jmp		Decisao				;pula pra ver se continua o loop

Decisao:	dec		R10						;decrementa o contador
			jz		M2M4_Fim				;se tiver zerado ir para o final
			inc		R5						;incrementa R5
			jmp		M2M4_Loop				;volta pro loop

M2T:		inc		R6						;incrementa o contador de multiplos de 2
			bit.b	#2,0(R5)				;testa se o numero e multiplo de 4
			jnc		M4T						;se n tiver carry o numero e multiplo de 4
			jmp 	Decisao					;vai para a decisao

M4T:		inc		R7						;incrementa o contador de multiplos de 4
			jmp 	Decisao					;vai para decisao

M2M4_Fim:	pop 	R11						;volta o contexto
			pop		R10						;volta contexto
			ret


			.data
vetor:		.byte	12, "ARIELGABRIEL" 		;declaraçao do vetor e seu conteudo

                                            

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
            

