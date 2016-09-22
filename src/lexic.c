#include "lexic.h"
#include <stdlib.h>

void lexicalAnalizer(FILE *input, FILE *output){
	char line[256];
	char buffer[256];
	int isCmt = 0;
	int i,j;

	while(fgets(line, 256, input) != NULL){
		i = 0;

		if(!isCmt && line[0] != '/' && line[1] != '*')	isCmt = 1;
		if(isCmt && line[0] != '*' && line[1] != '/')	isCmt = 0;

		if(!isCmt && line[0] != '/' && line[1] != '/'){

			while(line[i] != '\0' && line[i] != '\n'){
				while(line[i] == ' ' || line[i] == '\t')	i++;
				j = 0;
				while(line[i] != ' ' && line[i] != '\0' && line[i] != '\n'){
					buffer[j++] = line[i++];
				}
				buffer[j] = '\0';
				int out = automata(buffer, j);
				printf("%d ", out);
			}
			printf("\n");

		}
	}
}
