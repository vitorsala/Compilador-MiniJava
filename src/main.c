/*
 * main.c
 *
 *  Created on: 24 de ago de 2016
 *      Author: Kawai
 */

#include <stdio.h>
#include <stdlib.h>
#include "lexic.h"

int main(int argc, char* argv[]){
	FILE *input;
	FILE *output;

	if(argc == 1){
		printf("Quantidade de argumentos invalido.\n"
				"Uso: <executavel> <arq. de entrada>[ <arq. de saída>]");
		exit(-1);
	}
	if(argc >= 2){
		input = fopen(argv[1], "r");
		if(input == NULL){
			printf("Arquivo nao encontrado.");
			exit(-1);
		}
		if(argc == 3){
			output = fopen(argv[2], "w");
		}
		else{
			output = fopen("out.lxa", "w");
		}
		if(output == NULL){
			printf("Nao foi possivel criar o arquivo de saida.");
		}
	}

	lexicalAnalizer(input, output);

	fclose(input);
	fclose(output);

}
