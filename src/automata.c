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

int issymbol(char c) {
	return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ';' || c == '.' || c == ',';
}

int isop(char c) {
	return c == '=' || c == '<' || c == '!' || c == '+' || c == '-' || c == '*' || c == '&';
}

// Função o automato
int automata(char *str, int *index){
	int hasUnderscore = 0;
	int hasDot = 0;

	*index = 0;
	if(str[*index] == '\0')	return NONE;
	if(isalpha(str[*index]))	goto qid;
	if(isdigit(str[*index]))	goto qnum;
	if(issymbol(str[*index]))	goto qsym;
	if(isop(str[*index]))		goto qop;
	return NONE;

qid:
	while(str[*index] != '\0' && (isalpha(str[*index]) || str[*index] == '_' || str[*index] == '.')){
		if(str[*index] == '_')	hasUnderscore = 1;
		else if(str[*index] == '.' && hasDot == 0)	hasDot = *index;
		(*index)++;
	}

	if(!hasUnderscore){
		if(hasDot != 0 && strCmp(str, *index, reserved[13])) return RESERVED_PRINT;
		else{
			int i;
			if(hasDot != 0){
				*index = hasDot;
			}
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

qnum:
	while(str[*index] != '\0' && isdigit(str[*index]))	(*index)++;
	return NUMBER;
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
	default: return NONE; //Desnecessário?
	}

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
	return NONE; //Desnecessário?

}
