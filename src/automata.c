#include "automata.h"
#include <ctype.h>

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

int strCmp(const char *str1, int len, const char *str2){
	int i;
	for(i = 0; i < len; i++){
		if(str1[i] != str2[i])	return 0;
	}
	if(str2[i] != '\0')	return 0;
	return 1;
}

int automata(char *str, int len){
	int index = 0;
	int hasUnderscore = 0;
	int hasDot = 0;

	if(index >= len)	return NONE;
	if(isalpha(str[index]))	goto qid;
	if(isdigit(str[index]))	goto qnum;
	if(issymbol(str[index]))	goto qsym;
	if(isop(str[index]))		goto qop;
	return NONE;

qid:

	while(index < len && (isalpha(str[index]) || str[index] == '_' || str[index] == '.')){
		if(str[index] == '_')	hasUnderscore = 1;
		else if(str[index] == '.')	hasDot = 1;
		index++;
	}

	if(index == len){
		if(!hasUnderscore){
			if(hasDot && strCmp(str, len, reserved[13])) return RESERVED_PRINT;
			else{
				int i;
				for(i = 0; i < 20; i++){
					if(strCmp(str, len, reserved[i]))	return i+3;
				}
			}
		}
		if(!hasDot)	return ID;
	}
	return NONE;

qnum:
	while(index < len && isdigit(str[index]))	index++;
	if(index == len)	return NUMBER;
	else return NONE;
	qsym:
	if (len > 1)
		return NONE;
	else {
		switch (str[0]) {
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
	}

qop:
	if (len > 2)
		return NONE;
	if (len == 1) {
		switch (str[0]) {
		case '=': return OP_EQUAL;
		case '<': return OP_MINOR;
		case '+': return OP_PLUS;
		case '-': return OP_MINUS;
		case '*': return OP_MULT;
		case '!': return OP_NOT;
		default: return NONE; //Desnecessário?
		}
	}
	if (str[0] == '=' && str[1] == '=')
		return OP_DOUBLEEQUALS;
	if (str[0] == '!' && str[1] == '=')
		return OP_DIFFERENT;
	if (str[0] == '&' && str[1] == '&')
		return OP_AND;
	return NONE; //Desnecessário?

}

