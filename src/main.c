/*
 * main.c
 *
 *  Created on: 24 de ago de 2016
 *      Author: Kawai
 */

#include <stdio.h>
#include <stdlib.h>

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

int strCmp(const char *str1, int str1len, const char *str2);
int automata(char *str, int *end);

int main(int argc, char* argv[]){
	FILE *input;
	FILE *output;

	if(argc == 1){
//		printf("Quantidade de argumentos invalido.\n"
//				"Uso: <executavel> <arq. de entrada>[ <arq. de saída>]");
//		exit(-1);
        input = fopen("input.txt", "r");
        if(input == NULL){
            printf("Arquivo nao encontrado.");
            exit(-1);
        }
        output = fopen("output.txt", "w");
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
			output = fopen("output.txt", "w");
		}
		if(output == NULL){
			printf("Nao foi possivel criar o arquivo de saida.");
		}
	}

	lexicalAnalizer(input, output);

	fclose(input);
	fclose(output);

}
