#include "automata.h"
#include <ctype.h>

int isop(char c){
	return c == '+' || c == '-' || c == '*' || c == '/';
}

