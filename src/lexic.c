#include "lexic.h"

#include <stdlib.h>
#include "util.h"


char* getToken(int id, char* word, int len){
	int i;
	char* attr;
	if(id == ID){
		attr = (char*) calloc (len + 5, sizeof(char));
		char* trimmed = strtrim(word, len);

		// No caso do ID, é necessário verificar se o ID ja foi utilizado anteriormente
		// para a recuperação do atributo do token.
		i = 0;
		while(ids[i] != NULL){
			// Para cada ID já declarado anteriormente, verificar se o ID atual corresponde
			// a algum dos já declarados
			if(strCmp(trimmed, len, ids[i])){
				sprintf(attr,"<ID,%d>",i);
				return attr;
			}
			i++;
		}
		ids[i] = trimmed;
		sprintf(attr,"<ID,%d>",i);
		return attr;
	}
	else if(id == NUMBER){
		// Pequeno tratamento de string para adicionar o valor do dígito ao atributo.
		attr = (char*) calloc (len + 9, sizeof(char));
		sprintf(attr, "<NUMBER,%s>", strtrim(word, len));
		return attr;
	}
	else if(id == RESERVED_BOOL){
		return "<BOOL>";
	}
	else if(id == RESERVED_CLASS){
		return "<CLASS>";
	}
	else if(id == RESERVED_EXTENDS){
		return "<EXTENDS>";
	}
	else if(id == RESERVED_PUBLIC){
		return "<PUBLIC>";
	}
	else if(id == RESERVED_STATIC){
		return "<STATIC>";
	}
	else if(id == RESERVED_VOID){
		return "<VOID>";
	}
	else if(id == RESERVED_MAIN){
		return "<MAIN>";
	}
	else if(id == RESERVED_STRING){
		return "<STRING>";
	}
	else if(id == RESERVED_RETURN){
		return "<RETURN>";
	}
	else if(id == RESERVED_INT){
		return "<INT>";
	}
	else if(id == RESERVED_IF){
		return "<IF>";
	}
	else if(id == RESERVED_ELSE){
		return "<ELSE>";
	}
	else if(id == RESERVED_WHILE){
		return "<WHILE>";
	}
	else if(id == RESERVED_PRINT){
		return "<SYSTEM.OUT.PRINTLN>";
	}
	else if(id == RESERVED_LENGTH){
		return "<LENGTH>";
	}
	else if(id == RESERVED_TRUE){
		return "<TRUE>";
	}
	else if(id == RESERVED_FALSE){
		return "<FALSE>";
	}
	else if(id == RESERVED_THIS){
		return "<THIS>";
	}
	else if(id == RESERVED_NEW){
		return "<NEW>";
	}
	else if(id == RESERVED_NULL){
		return "<NULL>";
	}
	else if(id == SYMBOL_OP){
		return "<SYMBOL,OP>";
	}
	else if(id == SYMBOL_CP){
		return "<SYMBOL,CP>";
	}
	else if(id == SYMBOL_OB){
		return "<SYMBOL,OB>";
	}
	else if(id == SYMBOL_CB){
		return "<SYMBOL,CB>";
	}
	else if(id == SYMBOL_OCB){
		return "<SYMBOL,OCB>";
	}
	else if(id == SYMBOL_CCB){
		return "<SYMBOL,CCB>";
	}
	else if(id == SYMBOL_SEMICOLON){
		return "<SYMBOL,SEMICOLON>";
	}
	else if(id == SYMBOL_DOT){
		return "<SYMBOL,DOT>";
	}
	else if(id == SYMBOL_COMMA){
		return "<SYMBOL,COMMA>";
	}
	else if(id == OP_EQUAL){
		return "<OP,EQUAL>";
	}
	else if(id == OP_MINOR){
		return "<OP,MINOR>";
	}
	else if(id == OP_DOUBLEEQUALS){
		return "<OP,DOUBLEEQUALS>";
	}
	else if(id == OP_DIFFERENT){
		return "<OP,DIFFERENT>";
	}
	else if(id == OP_PLUS){
		return "<OP,PLUS>";
	}
	else if(id == OP_MINUS){
		return "<OP,MINUS>";
	}
	else if(id == OP_MULT){
		return "<OP,MULT>";
	}
	else if(id == OP_AND){
		return "<OP,AND>";
	}
	else if(id == OP_NOT){
		return "<OP,NOT>";
	}
	return "<ERROR>";
}

// Função do analisador léxico
void lexicalAnalizer(FILE *input, FILE *output){
	char line[MAX_STR_SIZE];
	char buffer[MAX_STR_SIZE];
	int isCmt = 0;
	int bufferIndex;
	int i,j,k;

	while(fgets(line, MAX_STR_SIZE, input) != NULL){
		i = 0;

		// Elimina os espaços em branco antes do texto.
		while(line[i] == ' ' || line[i] == '\t')	i++;

		// Verificação se não é um comentário de linha
		if(!(line[i] == '/' && line[i + 1] == '/')){
			while(line[i] != '\0' && line[i] != '\n'){
				// pula os espaços em branco.
				while(line[i] == ' ' || line[i] == '\t')	i++;

				// Começo do tratamento da cadeia que será verificado no autômato.
				j = 0;
				while(line[i] != ' ' && line[i] != '\0' && line[i] != '\n'){

					// Verificação de comentário dentro de uma cadeia de caracteres
					if(!isCmt && line[i] == '/' && line[i + 1] == '*')	isCmt = 1;
					if(isCmt){	// Se a cadeia estiver após o início de um comentário inline/multi-linha
						if(line[i] == '*' && line[i + 1] == '/'){	// Verifica o final da cadeia de comentário
							isCmt = 0;
							i += 2;
						}
						else	i++;
					}
					// Se não estiver dentro de um comentário, adicionar o caractere ao buffer
					else 		buffer[j++] = line[i++];
				}
				if(!isCmt){
					k = 0;
					bufferIndex = 0;
					buffer[j] = '\0';
					while(buffer[bufferIndex] != '\0' && buffer[bufferIndex] != '\n'){
						// Verifiação de uma palavra.
						int out = automata(buffer + bufferIndex, &k);
						// Dado uma palavra que foi reconhecida, pegar o token correspondente.
						char* str = getToken(out, buffer + bufferIndex, k);
						// output
						fprintf(stderr,"%s\n", str);
						fprintf(output,"%s\n", str);
						bufferIndex += k;
					}
				}
			}
		}
	}
}
