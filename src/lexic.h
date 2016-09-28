#ifndef LEXIC_H_
#define LEXIC_H_

#include "automata.h"
#include <stdio.h>

#define MAX_ID_QT 255
#define MAX_STR_SIZE 255

void lexicalAnalizer(FILE *input, FILE *output);
char* ids[MAX_ID_QT];

#endif /* LEXIC_H_ */
