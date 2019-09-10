; Pedro Augusto Ramalho Duarte 17016371
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
			mov		#vetor, R4
			call	#somaAcumulada
			jmp		$
			nop

somaAcumulada:
			mov.b	@R4+, R6				; Contador incializado com o tamanho do vetor
            inc		R6						; Incrementa o contador porque a verificação de tamanho é no começo do loop

            clr		R5
            clr		R7						; Auxliar de contagem

somaAcumulada_loop:

			dec		R6
			cmp.b	#0, R6
			jeq		somaAcumulada_fim

			mov.b	R5, R7					; Move R5 para o auxiliar da contagem
			mov.b	@R4+, R8				; Auxiliar de atribuição
			add.b	R8, R7					; Soma na auxiliar e incrementa posição do vetor

			BIT		#BIT8, R2
			jc		somaAcumulada_extrapolou

			;cmp.b	#0x7F, R7
			;jge     somaAcumulada_extrapola_cima		; Extrapolou

			;cmp.b	#0x80, R7
			;jl      somaAcumulada_extrapola_baixo		; Extrapolou

			mov.b	R7, R5
			jmp		somaAcumulada_loop

somaAcumulada_extrapolou:
			cmp.b	#0, R5
			jge     somaAcumulada_extrapola_cima
			jmp		somaAcumulada_extrapola_baixo

somaAcumulada_extrapola_cima:
			mov.b	#0x7F, R5
			jmp		somaAcumulada_loop

somaAcumulada_extrapola_baixo:
			mov.b	#0x80, R5
			jmp		somaAcumulada_loop

somaAcumulada_fim:
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
vetor:		.byte		9, -76, 77, -120, -32, -54, 0x7F, 161, 37, 92
