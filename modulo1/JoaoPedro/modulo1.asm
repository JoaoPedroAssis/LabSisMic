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
NUM			.equ	3804

			CLR		R7
			MOV		#NUM, R5
			MOV		#RESP, R6
			CALL	#ALG_ROM
			JMP		$
			NOP
;
ALG_ROM:	CMP 	#4000, R5
			JHS		MAIOR

			CMP		#1000, R5
			JHS		MAIOR1000

			CMP		#900, R5
			JHS		MAIOR900

			CMP		#500, R5
			JHS		MAIOR500

			CMP		#400, R5
			JHS		MAIOR400

			CMP		#100, R5
			JHS		MAIOR100

			CMP		#90, R5
			JHS		MAIOR90

			CMP		#50, R5
			JHS		MAIOR50

			CMP		#40, R5
			JHS		MAIOR40

			CMP		#10, R5
			JHS		MAIOR10

			CMP		#9, R5
			JHS		MAIOR9

			CMP		#5, R5
			JHS		MAIOR5

			CMP		#4, R5
			JHS		MAIOR4

			CMP		#1, R5
			JHS		MAIOR1

			RET

MAIOR:
			RET

MAIOR1000:	SUB		#1000, R5
			ADD		R7, R6
			MOV.B	#'M', 0(R6)
			SUB		R7, R6
			INC		R7
			JMP		ALG_ROM
			NOP

MAIOR900:
			SUB		#900, R5
			ADD		R7, R6
			MOV.B	#'C', 0(R6)
			MOV.B	#'M', 1(R6)
			SUB		R7, R6
			ADD		#2, R7
			JMP		ALG_ROM
			NOP
MAIOR500:
			SUB		#500, R5
			ADD		R7, R6
			MOV.B	#'D', 0(R6)
			SUB		R7, R6
			INC		R7
			JMP		ALG_ROM
			NOP
MAIOR400:
			SUB		#400, R5
			ADD		R7, R6
			MOV.B	#'C', 0(R6)
			MOV.B	#'D', 1(R6)
			SUB		R7, R6
			ADD		#2, R7
			JMP		ALG_ROM
			NOP
MAIOR100:
			SUB		#100, R5
			ADD		R7, R6
			MOV.B	#'C', 0(R6)
			SUB		R7, R6
			INC		R7
			JMP		ALG_ROM
			NOP
MAIOR90:
			SUB		#90, R5
			ADD		R7, R6
			MOV.B	#'X', 0(R6)
			MOV.B	#'C', 1(R6)
			SUB		R7, R6
			ADD		#2, R7
			JMP		ALG_ROM
			NOP
MAIOR50:
			SUB		#50, R5
			ADD		R7, R6
			MOV.B	#'L', 0(R6)
			SUB		R7, R6
			INC		R7
			JMP		ALG_ROM
			NOP
MAIOR40:
			SUB		#40, R5
			ADD		R7, R6
			MOV.B	#'X', 0(R6)
			MOV.B	#'L', 1(R6)
			SUB		R7, R6
			ADD		#2, R7
			JMP		ALG_ROM
			NOP
MAIOR10:
			SUB		#10, R5
			ADD		R7, R6
			MOV.B	#'X', 0(R6)
			SUB		R7, R6
			INC		R7
			JMP		ALG_ROM
			NOP
MAIOR9:
			SUB		#9, R5
			ADD		R7, R6
			MOV.B	#'I', 0(R6)
			MOV.B	#'X', 1(R6)
			SUB		R7, R6
			ADD		#2, R7
			JMP		ALG_ROM
			NOP
MAIOR5:
			SUB		#5, R5
			ADD		R7, R6
			MOV.B	#'V', 0(R6)
			SUB		R7, R6
			INC		R7
			JMP		ALG_ROM
			NOP
MAIOR4:
			SUB		#4, R5
			ADD		R7, R6
			MOV.B	#'I', 0(R6)
			MOV.B	#'V', 1(R6)
			SUB		R7, R6
			ADD		#2, R7
			JMP		ALG_ROM
			NOP
MAIOR1:
			SUB		#1, R5
			ADD		R7, R6
			MOV.B	#'I', 0(R6)
			SUB		R7, R6
			INC		R7
			JMP		ALG_ROM
			NOP


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
; data
;-------------------------------------------------------------------------------

			.data
RESP:		.byte	"----------------------------------", 0
