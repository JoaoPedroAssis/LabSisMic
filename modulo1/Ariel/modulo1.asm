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

NUM         .equ            2847                ;Indicar número a ser convertido
;
            mov             #NUM,R5             ;R5 = número a ser convertido
            mov             #RESP,R6            ;R6 = ponteiro para escrever a resposta
            call            #ALG_ROM            ;chamar subrotina
            jmp             $                   ;travar execução
            nop                                 ;exigido pelo montador
;



ALG_ROM:
Milhar:
            push            R7                  ;guardando contexto
            clr             R7                  ;zerando R7
            mov             #1000,R9            ;ajeitando divisor
            mov             R5,R8               ;ajeitando o dividendo
            call            #DIVISAO            ;chamando DIVISAO

            mov             R9,R5               ;atualizando o novo valor
            mov             R8,R7               ;colocoando o quociente no registro de contador

Loop_Milhar:
            add             #0,R7               ;atualizando flags para o R7
            jz              Centena             ;Pulando se o numero nao tiver valor na casa dos milhares ou acabar o loop
            mov.b           #0x4D,0(R6)         ;Colocando 'M' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            dec             R7                  ; decrementando contador de vezes que irei colocar a letra na memoria
            jmp             Loop_Milhar         ;jump para o inicio do loop


Centena:
            mov             #100,R9             ;ajeitando divisor
            mov             R5,R8               ;ajeitando o dividendo
            call            #DIVISAO            ;dividindo o resto por 100

            mov             R9,R5               ;atualizando o novo valor
            mov             R8,R7               ;colocoando o quociente no registro de contador

            cmp             #4,R7               ;primeiro if comparando pra ver se R7 e menor do 3
            jhs             Decisao_centena1    ;pulando para o else caso R7 >= 4

Loop_Centena1:
            add             #0,R7               ;atualizando flags para o R7
            jz              Dezena              ;saindo do loop
            mov.b           #0x43,0(R6)         ;Colocando 'C' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            dec             R7                  ;decrementando contador de vezes que irei colocar a letra na memoria
            jmp             Loop_Centena1       ;voltando para o incio do loop

Decisao_centena1:
            cmp             #4,R7               ;segundo if comparando pra ver se R7 == 4
            jne             Decisao_centena2    ;pulando para o else caso R7 != 4

Loop_Centena2:
            mov.b           #0x43,0(R6)         ;Colocando 'C' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            mov.b           #0x44,0(R6)         ;Colocando 'D' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            jmp             Dezena              ;indo para a proxima parte

Decisao_centena2:
            cmp             #9,R7               ;terceiro if comparando pra ver se R7 e menor do 9
            jhs             Decisao_centena4    ;pulando para o else caso R7 >= 9
            mov.b           #0x44,0(R6)         ;Colocando 'D' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            sub             #5,R7               ;descolando o contador


Loop_Centena3:
            add             #0,R7               ;atualizando flags para o R7
            jz              Dezena              ;saindo do loop
            mov.b           #0x43,0(R6)         ;Colocando 'C' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            dec             R7                  ;decrementando contador de vezes que irei colocar a letra na memoria
            jmp             Loop_Centena3       ;voltando para o incio do loop

Decisao_centena4:

Loop_Centena4:
            mov.b           #0x43,0(R6)         ;Colocando 'C' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            mov.b           #0x4D,0(R6)         ;Colocando 'M' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            jmp             Dezena              ;indo para a proxima parte

Dezena:
            mov             #10,R9              ;ajeitando divisor
            mov             R5,R8               ;ajeitando o dividendo
            call            #DIVISAO            ;dividindo o resto por 10

            mov             R9,R5               ;atualizando o novo valor
            mov             R8,R7               ;colocoando o quociente no registro de contador

            cmp             #4,R7               ;primeiro if comparando pra ver se R7 e menor do 3
            jhs             Decisao_Dezena1     ;pulando para o else caso R7 >= 4

Loop_Dezena1:
            add             #0,R7               ;atualizando flags para o R7
            jz              Unidade             ;saindo do loop
            mov.b           #0x58,0(R6)         ;Colocando 'X' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            dec             R7                  ;decrementando contador de vezes que irei colocar a letra na memoria
            jmp             Loop_Dezena1        ;voltando para o incio do loop

Decisao_Dezena1:
            cmp             #4,R7               ;segundo if comparando pra ver se R7 == 4
            jne             Decisao_Dezena2     ;pulando para o else caso R7 != 4

Loop_Dezena2:
            mov.b           #0x58,0(R6)         ;Colocando 'X' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            mov.b           #0x4C,0(R6)         ;Colocando 'L' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            jmp             Unidade             ;indo para a proxima parte

Decisao_Dezena2:
            cmp             #9,R7               ;terceiro if comparando pra ver se R7 e menor do 9
            jhs             Decisao_Dezena4  	;pulando para o else caso R7 >= 9
            mov.b           #0x4C,0(R6)         ;Colocando 'L' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            sub             #5,R7               ;descolando o contador


Loop_Dezena3:
            add             #0,R7               ;atualizando flags para o R7
            jz              Unidade             ;saindo do loop
            mov.b           #0x58,0(R6)         ;Colocando 'X' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            dec             R7                  ;decrementando contador de vezes que irei colocar a letra na memoria
            jmp             Loop_Dezena3        ;voltando para o incio do loop

Decisao_Dezena4:

Loop_Dezena4:
            mov.b           #0x58,0(R6)         ;Colocando 'X' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            mov.b           #0x43,0(R6)         ;Colocando 'C' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            jmp             Unidade             ;indo para a proxima parte


Unidade:	mov             #1,R9              ;ajeitando divisor
            mov             R5,R8               ;ajeitando o dividendo
            call            #DIVISAO            ;dividindo o resto por 10

            mov             R9,R5               ;atualizando o novo valor
            mov             R8,R7               ;colocoando o quociente no registro de contador

            cmp             #4,R7               ;primeiro if comparando pra ver se R7 e menor do 3
            jhs             Decisao_Unidade1   	;pulando para o else caso R7 >= 4


Loop_Unidade1:
            add             #0,R7               ;atualizando flags para o R7
            jz              Final             ;saindo do loop
            mov.b           #0x49,0(R6)         ;Colocando 'I' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            dec             R7                  ;decrementando contador de vezes que irei colocar a letra na memoria
            jmp             Loop_Unidade1        ;voltando para o incio do loop

Decisao_Unidade1:
            cmp             #4,R7               ;segundo if comparando pra ver se R7 == 4
            jne             Decisao_Unidade2    ;pulando para o else caso R7 != 4

Loop_Unidade2:
            mov.b           #0x49,0(R6)         ;Colocando 'I' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            mov.b           #0x56,0(R6)         ;Colocando 'V' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            jmp             Final             ;indo para a proxima parte

Decisao_Unidade2:
            cmp             #9,R7               ;terceiro if comparando pra ver se R7 e menor do 9
            jhs             Decisao_Unidade4    ;pulando para o else caso R7 >= 9
            mov.b           #0x56,0(R6)         ;Colocando 'V' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            sub             #5,R7               ;descolando o contador


Loop_Unidade3:
            add             #0,R7               ;atualizando flags para o R7
            jz              Final             ;saindo do loop
            mov.b           #0x49,0(R6)         ;Colocando 'I' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            dec             R7                  ;decrementando contador de vezes que irei colocar a letra na memoria
            jmp             Loop_Unidade3        ;voltando para o incio do loop

Decisao_Unidade4:

Loop_Unidade4:
            mov.b           #0x49,0(R6)         ;Colocando 'I' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            mov.b           #0x58,0(R6)         ;Colocando 'X' na posição de memoria do R6
            inc             R6                  ;avançando a poscição do ponteiro de escrita
            jmp             Final               ;indo para a proxima parte


Final:
			mov.b           #0x00,0(R6)			;Adicionando o final da string
			pop				R7
			ret






DIVISAO:    ;Recebe dividendo no R8 e divisor no R9, saida do quociente no R8 e do resto no R9
            push            R10
            clr             R10
DIVI_LOOP:
            inc             R10                 ;incrementando R10
            sub             R9,R8               ;subtraindo R9 de R8
            jn              DIVI_SAIU           ;caso o resultdo anterior seje negativo sair do loop
            jmp             DIVI_LOOP           ;jump incondicional

DIVI_SAIU:
            dec             R10                 ;ajustando o quociente pois foi uma contagem a mais
            add             R9,R8               ;ajeitando resto
            mov             R8,R9               ;colocando o resto no R8
            mov             R10,R8              ;colocando o quociente no R9
            pop             R10                 ;retaurando contexto
            ret







 ret
            .data
                                            ; Local para armazenar a resposta (RESP = 0x2400)
RESP:       .byte           "RRRRRRRRRRRRRRRRRR",0


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
            
