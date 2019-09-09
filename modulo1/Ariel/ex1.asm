main:           mov         #vetor,R5           ;Copiando o endereço do vetor
                call        #Menor              ;Chamando subrotina Menor
                jmp         $                   ;loop eterno para para travar o programa


Menor:			push		R10					;Salvando contexto

				mov.b		@R5+,R10			;Salvando o tamanho do vetor
				mov.b		#0xFF,R6			;Colocando o maior valor no R6

MenorLoop:
				cmp.b		R6,0(R5)			;comparando
				jlo			EhMenor				;vendo se R5  < R6
				jeq			EhIgual				;vendo see R5 == R6
				jmp			Decisao				;puloa para a parte da decisao sobre sair do loop

EhMenor:		mov			#1,R7				;Zerando contador de frequencia
				mov.b		@R5,R6				;Colocando o valor em R6
				jmp			Decisao				;indo para decisão

EhIgual:		inc			R7					;incrementando R7
				jmp			Decisao				;indo pra decisao

Decisao:		inc			R5					;avançando o contador
				dec			R10					;decrementando contador
				jz			MenorFim			;indo pro final
				jmp			MenorLoop			;voltando para o inicio do loop

MenorFim:		pop			R10					;restaurando contexto
				ret								;retornando



                .data
vetor:          .byte       12,"ARIELGABRIEL"       ;Declarando string
