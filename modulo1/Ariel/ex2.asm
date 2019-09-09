main:           mov         #vetor,R5           ;Copiando o endereço do vetor
                call        #Maior16            ;Chamando subrotina Menor
                jmp         $                   ;loop eterno para para travar o programa




Maior16:		push		R10					;Salvando contexto

				mov			@R5+,R10			;Salvando o tamanho do vetor
				mov			#0x0000,R6			;Colocando o menor valor no R6


Maior16Loop:
				cmp			R6,0(R5)			;comparando
				jhs			EhMaiorOuIgual		;vendo se R5  > R6
				jmp			Decisao				;puloa para a parte da decisao sobre sair do loop


EhMaiorOuIgual:
				jeq			EhIgual				;verificando se é igual
				mov			#1,R7				;Zerando contador de frequencia
				mov			@R5,R6				;Colocando o valor em R6
				jmp			Decisao				;indo para decisão

EhIgual:		inc			R7					;incrementando R7
				jmp			Decisao				;indo pra decisao


Decisao:		incd		R5					;avançando o contador
				dec			R10					;decrementando contador
				jz			MenorFim			;indo pro final
				jmp			Maior16Loop			;voltando para o inicio do loop


MenorFim:		pop			R10					;restaurando contexto
				ret								;retornando









                .data
vetor:          .byte       6,0,"ARIELGABRIEL"       ;Declarando string
                                            