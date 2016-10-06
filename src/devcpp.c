/*
 * main.c
 *
 *  Created on: 24 de ago de 2016
 *      Author: Kawai
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

#define MAX_ID_QT 255
#define MAX_STR_SIZE 255

// Lista de palavras reservadas.
const char* reserved[] = {
    "boolean",
    "class",
    "extends",
    "public",
    "static",
    "void",
    "main",
    "String",
    "return",
    "int",
    "if",
    "else",
    "while",
    "System.out.println", // 13
    "length",
    "true",
    "false",
    "this",
    "new",
    "null"
};

enum recognizedState{
  NONE,
  ID,               //ID
  NUMBER,           //Number
  RESERVED_BOOLEAN, //boolean
  RESERVED_CLASS,   //Class
  RESERVED_EXTENDS, //extends
  RESERVED_PUBLIC,  //public
  RESERVED_STATIC,  //static
  RESERVED_VOID,    //void
  RESERVED_MAIN,    //main
  RESERVED_STRING,  //String
  RESERVED_RETURN,  //return
  RESERVED_INT,     //int
  RESERVED_IF,      //if
  RESERVED_ELSE,    //else
  RESERVED_WHILE,   //while
  RESERVED_PRINT,   //System.out.println
  RESERVED_LENGTH,  //length
  RESERVED_TRUE,    //true
  RESERVED_FALSE,   //false
  RESERVED_THIS,    //this
  RESERVED_NEW,     //new
  RESERVED_NULL,    //null
  SYMBOL_OP,        //Open parenthesis
  SYMBOL_CP,        //Close parenthesis
  SYMBOL_OB,        //Open bracket
  SYMBOL_CB,        //Close bracket
  SYMBOL_OCB,       //Open curly bracket
  SYMBOL_CCB,       //Close curly bracket
  SYMBOL_SEMICOLON, //;
  SYMBOL_DOT,       //.
  SYMBOL_COMMA,     //,
  OP_EQUAL,         //=
  OP_MINOR,         //<
  OP_DOUBLEEQUALS,  //==
  OP_DIFFERENT,     //!=
  OP_PLUS,          //+
  OP_MINUS,         //-
  OP_MULT,          //*
  OP_AND,           //&&
  OP_NOT            //!
};

void lexicalAnalizer(FILE *input, FILE *output);
int automata(char *str, int *end);

int strCmp(const char *str1, int str1len, const char *str2);
char* strtrim(char* s, int len);

char* ids[MAX_ID_QT];


int main(int argc, char* argv[]){
	FILE *input;
	FILE *output;

    input = fopen("input.txt", "r");
    if(input == NULL){
        printf("Arquivo nao encontrado.");
        exit(-1);
    }
    output = fopen("output.txt", "w");

	lexicalAnalizer(input, output);

	fclose(input);
	fclose(output);
}


// =======================================================================================
// Analisador Lexico
// =======================================================================================


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
	else if(id == RESERVED_BOOLEAN){
		return "<BOOLEAN>";
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
						// Verificação de uma palavra.
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

// =======================================================================================
// Automato
// =======================================================================================

//Verifica se o character é um símbolo
int issymbol(char c) {
	return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ';' || c == '.' || c == ',';
}

//Verifica se o caracter é um operador
int isop(char c) {
	return c == '=' || c == '<' || c == '!' || c == '+' || c == '-' || c == '*' || c == '&';
}

// Função o automato
int automata(char *str, int *index){
	int hasUnderscore = 0;
	int hasDot = 0;

    //Verifica o primeiro caracter da palavra e muda para o estado correspondente, ou retorna NONE caso o caracter não seja reconhecido
	*index = 0;
	if(str[*index] == '\0')	return NONE;
	if(isalpha(str[*index]))	goto qid;   //Vai para o estado id caso o caracter seja alfabético
	if(isdigit(str[*index]))	goto qnum;  //Vai para o estado numeral caso o caracter seja um dígito
	if(issymbol(str[*index]))	goto qsym;  //Vai para o estado símbolo caso o caracter seja um símbolo
	if(isop(str[*index]))		goto qop;   //Vai para o estado operador caso o caracter seja um operador
	(*index)++;
	return NONE;

//Estado id, que verifica por ids ou palavras reservadas
qid:
    //Verifica se a palavra possui um ponto ou underscore para auxiliar o reconhecimento do token
    while(str[*index] != '\0' && (isalpha(str[*index]) || (isdigit(str[*index])) || str[*index] == '_' || str[*index] == '.')){
        printf("str: %c\n", str[*index]);
		if(str[*index] == '_')	hasUnderscore = 1;
        //Caso um ponto seja reconhecido, guarda na variável index, que foi passada por referência, a posição do ponto
		else if(str[*index] == '.' && hasDot == 0)	hasDot = *index;
		(*index)++;
	}

    //Caso não tenha underscore...
	if(!hasUnderscore){
        //Caso tenha ponto e a palavra seja equivalente a palavra reservada PRINT, retorna PRINT
		if(hasDot != 0 && strCmp(str, *index, reserved[13])) return RESERVED_PRINT;
		else{
			int i;
            //Caso tenha ponto mas a palavra não for reconhecida como PRINT, guarda na variável index, que foi passada por referência, a posição do ponto
			if(hasDot != 0){
				*index = hasDot;
			}
            //
			for(i = 0; i < 20; i++){
				if(strCmp(str, *index, reserved[i]))	return i+3;
			}
		}
		return ID;
	}
	else{
		if(hasDot != 0){
			*index = hasDot;
		}
		return ID;
	}
	return NONE;

//Estado numeral
qnum:
	while(str[*index] != '\0' && isdigit(str[*index]))	(*index)++;
	return NUMBER;
//Estado símbolo
qsym:
	(*index)++;
	switch (str[(*index) - 1]) {
	case '(': return SYMBOL_OP;
	case ')': return SYMBOL_CP;
	case '[': return SYMBOL_OB;
	case ']': return SYMBOL_CB;
	case '{': return SYMBOL_OCB;
	case '}': return SYMBOL_CCB;
	case ';': return SYMBOL_SEMICOLON;
	case '.': return SYMBOL_DOT;
	case ',': return SYMBOL_COMMA;
	default: return NONE;
	}

//Estado operador
qop:
	(*index)++;
	switch (str[*index - 1]) {
	case '=':
		if(str[*index] != '=')	return OP_EQUAL;
		else{
			(*index)++;
			return OP_DOUBLEEQUALS;
		}
	case '<': return OP_MINOR;
	case '+': return OP_PLUS;
	case '-': return OP_MINUS;
	case '*': return OP_MULT;
	case '!':
		if(str[*index] != '=')	return OP_NOT;
		else{
			(*index)++;
			return OP_DIFFERENT;
		}
	case '&':
		if(str[*index] != '&')	return OP_AND;
	}
	return NONE;

}

// Utilidades

//Método de comparação de Strings
int strCmp(const char *str1, int str1len, const char *str2){
	int i;
	for(i = 0; i < str1len || str2[i] != '\0'; i++){
		if(str1[i] != str2[i])	return 0;
	}
	if(i < str1len && str2[i] != '\0')	return 0;
	return 1;
}

//Método que remove os espaços vazios de uma String
char* strtrim(char* s, int len){
	int i = 0;
	char *out = (char*) calloc (len + 1, sizeof(char));
	for(;i < len; i++){
		out[i] = s[i];
	}
	out[i] = '\0';
	return out;
}

