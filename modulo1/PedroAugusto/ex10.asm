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

;-------------------------------------------------------------------------------
; Resultado Esperado (17016 - 0x4278)  0x34, 0x32, 0x37, 0x38
;-------------------------------------------------------------------------------
MATR		.set	17016

main:
			mov		#resposta, R5
			mov		#MATR, R6
			call	#W16_ASC
			jmp		$
			nop

W16_ASC:
			add		#6, R5					; Vai preenchendo o vetor de tras para frente
			mov		R6, R8					; Salva a informação do numero
			clr		R9						; Contandor
			BIC		#0xFFF0, R6				; Limpa os bits mais significativos do R6
			mov		R6, R7					; Usa a subrtoina NIB_ASC
			call	#NIB_ASC
			mov		R7, 0(R5)				; Escreve o nibble da memoria
			decd	R5 						; Inscrementa o nibble da memoria

W16_ASC_loop:

			mov		R8, R6					; Recupera o valor do número
			RRA		R6						; Move um byte para direta
			RRA		R6
			RRA		R6
			RRA		R6
			mov		R6, R8
			BIC		#0xFFF0, R6				; Limpa os bits mais significativos do R6
			mov		R6, R7					; Usa a subrtoina NIB_ASC
			call	#NIB_ASC
			mov		R7, 0(R5)				; Escreve o nibble da memoria
			decd	R5 						; Inscrementa o nibble da memoria
			inc		R9
			cmp		#3, R9					; Repete 3 vezes, já que fez uma na principal
			jne		W16_ASC_loop
			RET

; Recebe em R7 um nibble e retorna o valor ascii em R7
NIB_ASC:
			cmp	#0, R7
			jeq NIB_ASC_0
			cmp	#1, R7
			jeq NIB_ASC_1
			cmp	#2, R7
			jeq NIB_ASC_2
			cmp	#3, R7
			jeq NIB_ASC_3
			cmp	#4, R7
			jeq NIB_ASC_4
			cmp	#5, R7
			jeq NIB_ASC_5
			cmp	#6, R7
			jeq NIB_ASC_6
			cmp	#7, R7
			jeq NIB_ASC_7
			cmp	#8, R7
			jeq NIB_ASC_8
			cmp	#9, R7
			jeq NIB_ASC_9
			cmp	#10, R7
			jeq NIB_ASC_A
			cmp	#11, R7
			jeq NIB_ASC_B
			cmp	#12, R7
			jeq NIB_ASC_C
			cmp	#13, R7
			jeq NIB_ASC_D
			cmp	#14, R7
			jeq NIB_ASC_E
			cmp	#15, R7
			jeq NIB_ASC_F

NIB_ASC_0:
			mov #0x30, R7
			RET
NIB_ASC_1:
			mov #0x31, R7
			RET
NIB_ASC_2:
			mov #0x32, R7
			RET
NIB_ASC_3:
			mov #0x33, R7
			RET
NIB_ASC_4:
			mov #0x34, R7
			RET
NIB_ASC_5:
			mov #0x35, R7
			RET
NIB_ASC_6:
			mov #0x36, R7
			RET
NIB_ASC_7:
			mov #0x37, R7
			RET
NIB_ASC_8:
			mov #0x38, R7
			RET
NIB_ASC_9:
			mov #0x39, R7
			RET
NIB_ASC_A:
			mov #0x41, R7
			RET
NIB_ASC_B:
			mov #0x42, R7
			RET
NIB_ASC_C:
			mov #0x43, R7
			RET
NIB_ASC_D:
			mov #0x44, R7
			RET
NIB_ASC_E:
			mov #0x45, R7
			RET
NIB_ASC_F:
			mov #0x46, R7
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

			.data

resposta:	.word	1,2
