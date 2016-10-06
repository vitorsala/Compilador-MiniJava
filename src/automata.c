#include "automata.h"
#include <ctype.h>
#include "util.h"

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
