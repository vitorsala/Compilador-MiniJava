/*
 * main.c
 *
 *  Created on: 24 de ago de 2016
 *      Author: Kawai
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <curses.h>
#include <string.h>

#define MAX_ID_QT 255           //Tamanho máximo de um ID
#define MAX_STR_SIZE 255        //Tamanho máximo de uma String
#define MAX_CHILD_NODES_QT 17 //17 nós filhos, quantidade determinada pelo maior número de tokens em uma produção (Nesse caso 17 tokens de MAIN)

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

//Representa os tokens reconhecíveis
typedef enum {
    NONE,             //None
    ERROR,            //Error
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
    OP_NOT,            //!
    NUMBEROFSTATES    //Número total de estados
} RecognizedState;

//Vetor para mapear o número do enum com a String adequada (e.g., recognizedStates[ID] = "<ID>")
char *recognizedStates[] = {
    "<NONE>",
    "<ERROR>",
    "<ID>",
    "<NUMBER>",
    "<BOOLEAN>",
    "<CLASS>",
    "<EXTENDS>",
    "<PUBLIC>",
    "<STATIC>",
    "<VOID>",
    "<MAIN>",
    "<STRING>",
    "<RETURN>",
    "<INT>",
    "<IF>",
    "<ELSE>",
    "<WHILE>",
    "<PRINT>",
    "<LENGTH>",
    "<TRUE>",
    "<FALSE>",
    "<THIS>",
    "<NEW>",
    "<NULL>",
    "<SYMBOL_OP>",
    "<SYMBOL_CP>",
    "<SYMBOL_OB>",
    "<SYMBOL_CB>",
    "<SYMBOL_OCB>",
    "<SYMBOL_CCB>",
    "<SYMBOL_SEMICOLON>",
    "<SYMBOL_DOT>",
    "<SYMBOL_COMMA>",
    "<OP_EQUAL>",
    "<OP_MINOR>",
    "<OP_DOUBLEEQUALS>",
    "<OP_DIFFERENT>",
    "<OP_PLUS>",
    "<OP_MINUS>",
    "<OP_MULT>",
    "<OP_AND>",
    "<OP_NOT>"
};

//Representa as produções (regras) da gramática
typedef enum {
    EMPTY,
    LEAF,
    PROG,
    MAIN,
    CLASSE,
    VAR,
    METODO,
    PARAMS,
    TIPO,
    CMD,
    EXP,
    REXP,
    AEXP,
    MEXP,
    SEXP,
    PEXP,
    EXPS,
    LETRA,
    DIGITO,
    NT_ID,  //Node Type ID (Já existe o enumerador ID)
    NUM
} NodeType;

char *nodeTypes[] = {
    "EMPTY",
    "LEAF",
    "PROG",
    "MAIN",
    "CLASSE",
    "VAR",
    "METODO",
    "PARAMS",
    "TIPO",
    "CMD",
    "EXP",
    "REXP",
    "AEXP",
    "MEXP",
    "SEXP",
    "PEXP",
    "EXPS",
    "LETRA",
    "DIGITO",
    "ID",
    "NUM"
};

//Estrutura que representa os tokens lidos da lista produzida pela análise léxica
typedef struct TokenTag {
    RecognizedState recognizedState; //Tipo do token
    char value[MAX_STR_SIZE]; //Valor do token em caso de ID ou NUMBER
    int linenum; //Número da linha do token lido
    struct TokenTag *next; //Estrutura de lista ligada para a análise sintática
} TokenTag;

//Estrutura de nó para a árvore sintática
typedef struct Node {
    NodeType nodeType; //Tipo do nó
//    TokenTag *tokens; //Lista ligada de tokens deste nó (Desnecessário?)
    struct Node *childNodes[MAX_CHILD_NODES_QT]; //Nós filhos deste nó
    RecognizedState token; //Caso o nó seja uma folha, guarda apenas o tipo de token
} Node;

void lexicalAnalizer(FILE *input, FILE *output);
int automata(char *str, int *end);

void parser(FILE *input, FILE *output);
void printParseTree(FILE *output, Node *root, int level);
RecognizedState getRecognizedState(char *token);

Node *pProg();
Node *pMain();
Node *pClasse();
Node *pVar();
Node *pMetodo();
Node *pParams();
Node *pTipo();
Node *pCmd();
Node *pExp();
Node *pRexp();
Node *pAexp();
Node *pMexp();
Node *pSexp();
Node *pPexp();
Node *pExps();

int strCmp(const char *str1, int str1len, const char *str2);
//char* strtrim(char* s, int len);
char* strtrim(char* str);

char* ids[MAX_ID_QT];
TokenTag *tokenTag;


int main(int argc, char* argv[]){
	FILE *input;
	FILE *output;

    input = fopen("input.txt", "r");
    if(input == NULL){
        printf("Arquivo input.txt nao encontrado.\n");
        exit(-1);
    }
    output = fopen("tokenlist.txt", "w");

	lexicalAnalizer(input, output);

	fprintf(stderr,"Analise lexica finalizada");
	getchar();
    
    fclose(input);
    fclose(output);

    input = fopen("tokenlist.txt", "r");
    if(input == NULL){
        printf("Arquivo tokenlist.txt nao encontrado.\n");
        exit(-1);
    }
    output = fopen("parsetree.txt", "w");
    
    parser(input, output);
    
    fprintf(stderr,"Analise sintatica finalizada");
    getchar();
    
    fclose(input);
    fclose(output);
}


// =======================================================================================
// Analisador Léxico
// =======================================================================================

//Retorna um token com base no id encontrado pelo autômato
char* getToken(int id, char* word, int len){
	int i;
	char* attr;
	if(id == ID){
        attr = (char*) calloc (len + 5, sizeof(char));
//        char* trimmed = strtrim(word, len);
        char* trimmed = strtrim(word);

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
//        sprintf(attr, "<NUMBER,%s>", strtrim(word, len));
        sprintf(attr, "<NUMBER,%s>", strtrim(word));
		return attr;
	}
    else if (id > 2 && id < NUMBEROFSTATES) {
        return recognizedStates[id];
    }
	return recognizedStates[ERROR];
}

// Função do analisador léxico
void lexicalAnalizer(FILE *input, FILE *output){
	char line[MAX_STR_SIZE];
	char buffer[MAX_STR_SIZE];
	int isCmt = 0;
	int bufferIndex;
	int i,j,k;
	int linenum = 0;

	while(fgets(line, MAX_STR_SIZE, input) != NULL){
		linenum++;
		i = 0;

		// Elimina os espaços em branco antes do texto.
		while(line[i] == ' ' || line[i] == '\t')	i++;

		// Verificação se não é um comentário de linha
		if(!(line[i] == '/' && line[i + 1] == '/')){
			while(line[i] != '\0' && line[i] != '\n'){
				// pula os espaços em branco.
				while(line[i] == ' ' || line[i] == '\t')	i++;

				// Começo do tratamento da cadeia que será verificado no automato.
				j = 0;
				while(line[i] != ' ' && line[i] != '\0' && line[i] != '\n'){

					if(!isCmt && line[i] == '/' && line[i + 1] == '/'){
						line[i] = '\0';
						break;
					}

					// Verificação de comentário dentro de uma cadeia de caracteres
					if(!isCmt && line[i] == '/' && line[i + 1] == '*'){
						if(j > 0){
							break;
						}
						else{
							isCmt = 1;
						}
					}
					if(isCmt){	// Se a cadeia estiver após o início de um comentário inline/multi-linha
						if(line[i] == '*' && line[i + 1] == '/'){	// Verifica o final da cadeia de coment?rio
							isCmt = 0;
							i += 2;
						}
						else	i++;
					}
					// Se não estiver dentro de um comentário, adicionar o caractere ao buffer
					else 		buffer[j++] = line[i++];
				}
				if(!isCmt && j > 0){
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

						if(out == ERROR){
							printf("Erro de reconhecimento lexico na linha %d\n", linenum);
							return;
						}

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

    //Verifica o primeiro caracter da palavra e muda para o estado correspondente, ou retorna ERROR caso o caracter não seja reconhecido
	*index = 0;
	if(str[*index] == '\0')	return ERROR;
	if(isalpha(str[*index]))	goto qid;   //Vai para o estado id caso o caractere seja uma letra
	if(isdigit(str[*index]))	goto qnum;  //Vai para o estado numeral caso o caracter seja um dígito
	if(issymbol(str[*index]))	goto qsym;  //Vai para o estado símbolo caso o caracter seja um símbolo
	if(isop(str[*index]))		goto qop;   //Vai para o estado operador caso o caracter seja um operador
    //printf("str: %c\n", str[*index]);
	(*index)++;
	return ERROR;

//Estado id, que verifica por ids ou palavras reservadas
qid:
    //Verifica se a palavra possui um ponto ou underscore para auxiliar o reconhecimento do token
    while(str[*index] != '\0' && (isalpha(str[*index]) || (isdigit(str[*index])) || str[*index] == '_' || str[*index] == '.')){
        //printf("str: %c\n", str[*index]);
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
				if(strCmp(str, *index, reserved[i]))	return i+4;
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
	return ERROR;

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
	default: return ERROR;
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
	return ERROR;

}

// =======================================================================================
// Analisador Sintático
// =======================================================================================

//Retorna um enum de token com base na String lida (e.g., "<ID>" retorna 1)
RecognizedState getRecognizedState(char *token) {
    //WARNING: i = 1 pula o token <Error>
    for (int i = 1; i < NUMBEROFSTATES; i++) {
        if (strcmp(recognizedStates[i], token) == 0)
            return i;
    }
    return ERROR;
}

//Função do analisador sintático
void parser(FILE *input, FILE *output){
    char line[MAX_STR_SIZE];
    int linenum = 0;
    tokenTag = malloc(sizeof(TokenTag));  //Token Tag guarda as informações do token lido
    TokenTag *head = tokenTag;  //Ponteiro que aponta para o primeiro token lido
    
    while(fgets(line, MAX_STR_SIZE, input) != NULL){
        linenum++;
        tokenTag->linenum = linenum;
        
        char *token = strtrim(line); //Remove os espaços vazios e cria um ponteiro que aponta para o token lido (Necessário para o método strsep)
        
//        //O bloco abaixo remove os sinais de '<' e '>' do token
//        int toklen = strlen(token); //Tamanho do token
//        memmove(&token[0], &token[1], toklen);          //Remove o primeiro caracter da String
//        memmove(&token[toklen-2], &token[toklen-1], 1); //Remove o último caracter da String
        
        /*
         Separa o token no delimitador "," (Presente em ID e NUM) e envia a primeira metade para o método getRecognizedState, que retorna o tipo do token
         Caso não exista o delimitador ",", é enviado todo o token para o método getRecognizedState
         O tipo de token é atribuído ao Token Tag
         */
        
        //WARNING: Revisar o código abaixo
        if (strpbrk(token, ",") != NULL) {
            tokenTag->recognizedState = getRecognizedState(strcat(strsep(&token, ","), ">"));
            //O bloco abaixo remove os sinais de '<' e '>' do token
            int toklen = strlen(token); //Tamanho do token
            memmove(&token[toklen-1], &token[toklen], 1); //Remove o último caracter da String
            strcpy(tokenTag->value, token); //Atribui o valor do token em Token Tag
        } else {
            tokenTag->recognizedState = getRecognizedState(token);
        }
        
        //WARNING: Cria um nó a mais, que fica vazio
        tokenTag->next = malloc(sizeof(TokenTag));
        tokenTag = tokenTag->next;
        tokenTag->next = NULL;
    }
    
    tokenTag = head;
    Node *root = pProg();
    printParseTree(output, root, 0);
    
    //Libera o espaço de memória ocupado pelos Token Tags
    TokenTag *curr;
    while ((curr = head) != NULL) {
        head = head->next;
        free(curr);
    }
}

// =======================================================================================
// Árvore Sintática
// =======================================================================================

//Retorna um nó vazio do tipo especificado
Node *getNode(NodeType nodeType) {
    Node *node = malloc(sizeof(Node));
    node->nodeType = nodeType;
    node->token = 0;
    for (int i = 0; i < MAX_CHILD_NODES_QT; i++)
        node->childNodes[i] = NULL;
    return node;
}

//Retorna uma folha com o token especificado
Node *getLeaf(RecognizedState token) {
    Node *node = malloc(sizeof(Node));
    node->nodeType = LEAF;
    node->token = token;
    for (int i = 0; i < MAX_CHILD_NODES_QT; i++)
        node->childNodes[i] = NULL;
    return node;
}

void printParseTree(FILE *output, Node *root, int level) {
    if (root->token != 0)
        fprintf(output, "%*s" "%s\n", level * 4, "", recognizedStates[root->token]);
    else {
        fprintf(output, "%*s" "%s\n", level * 4, "", nodeTypes[root->nodeType]);
        level++;
        for (int i = 0; root->childNodes[i] != NULL; i++)
            printParseTree(output, root->childNodes[i], level);
    }
    free(root);
}

Node *pProg() {
    Node *node = getNode(PROG);
    node->childNodes[0] = pMain();
    
    node->childNodes[1] = pClasse();

    return node;
}

Node *pMain() {
    Node *node = getNode(MAIN);
    
    int expected = RESERVED_CLASS;
    if (tokenTag->recognizedState == expected)
        node->childNodes[0] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = ID;
    if (tokenTag->recognizedState == expected)
        node->childNodes[1] = getLeaf(ID);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = SYMBOL_OCB;
    if (tokenTag->recognizedState == expected)
        node->childNodes[2] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = RESERVED_PUBLIC;
    if (tokenTag->recognizedState == expected)
        node->childNodes[3] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = RESERVED_STATIC;
    if (tokenTag->recognizedState == expected)
        node->childNodes[4] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = RESERVED_VOID;
    if (tokenTag->recognizedState == expected)
        node->childNodes[5] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = RESERVED_MAIN;
    if (tokenTag->recognizedState == expected)
        node->childNodes[6] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = SYMBOL_OP;
    if (tokenTag->recognizedState == expected)
        node->childNodes[7] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = RESERVED_STRING;
    if (tokenTag->recognizedState == expected)
        node->childNodes[8] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);

    tokenTag = tokenTag->next;
    expected = SYMBOL_OB;
    if (tokenTag->recognizedState == expected)
        node->childNodes[9] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    tokenTag = tokenTag->next;
    expected = SYMBOL_CB;
    if (tokenTag->recognizedState == expected)
        node->childNodes[10] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    tokenTag = tokenTag->next;
    expected = ID;
    if (tokenTag->recognizedState == expected)
        node->childNodes[11] = getLeaf(ID);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    tokenTag = tokenTag->next;
    expected = SYMBOL_CP;
    if (tokenTag->recognizedState == expected)
        node->childNodes[12] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    tokenTag = tokenTag->next;
    expected = SYMBOL_OCB;
    if (tokenTag->recognizedState == expected)
        node->childNodes[13] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    tokenTag = tokenTag->next;
    node->childNodes[14] = pCmd();
    
    expected = SYMBOL_CCB;
    if (tokenTag->recognizedState == expected)
        node->childNodes[15] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    tokenTag = tokenTag->next;
    expected = SYMBOL_CCB;
    if (tokenTag->recognizedState == expected)
        node->childNodes[16] = getLeaf(expected);
    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    tokenTag = tokenTag->next;
    return node;
}

Node *pClasse() {
    if (tokenTag->recognizedState == NONE)
        return NULL;
    
//    Node *node = getNode(CLASSE);
//    int childCount = -1;
//    
//    int expected = RESERVED_CLASS;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    expected = ID;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(ID);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    expected = RESERVED_EXTENDS;
//    if (tokenTag->recognizedState == expected) {
//        node->childNodes[childCount++] = getLeaf(expected);
//        
//        tokenTag = tokenTag->next;
//        expected = ID;
//        if (tokenTag->recognizedState == expected)
//            node->childNodes[childCount++] = getLeaf(ID);
//        else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    } else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    expected = SYMBOL_OCB;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    //WARNING: Problema de produção gramatical
//    tokenTag = tokenTag->next;
//    node->childNodes[childCount++] = pVar();
//    
//    //WARNING: Problema de produção gramatical
//    node->childNodes[childCount++] = pMetodo();
//    
//    expected = SYMBOL_CCB;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    expected = SYMBOL_CCB;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    if (tokenTag != NULL)
//        node->childNodes[childCount++] = pClasse();
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pVar() {
//    Node *node = getNode(VAR);
//    
//    node->childNodes[0] = pTipo();
//    
//    node->childNodes[1] = getLeaf(ID);
//    
//    tokenTag = tokenTag->next;
//    int expected = SYMBOL_SEMICOLON;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[2] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
    
    //WARNING: O que fazer em caso de múltiplos VAR?
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pMetodo() {
//    Node *node = getNode(METODO);
//    int childCount = -1;
//    
//    int expected = RESERVED_PUBLIC;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    node->childNodes[childCount++] = pTipo();
//    
//    node->childNodes[childCount++] = pId();
//    
//    int expected = SYMBOL_OP;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    node->childNodes[childCount++] = pParams();
//    
//    int expected = SYMBOL_CP;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    int expected = SYMBOL_OCB;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    node->childNodes[childCount++] = pVar();
//    
//    tokenTag = tokenTag->next;
//    node->childNodes[childCount++] = pCmd();
//    
//    int expected = RESERVED_RETURN;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    node->childNodes[childCount++] = pExp();
//    
//    tokenTag = tokenTag->next;
//    int expected = SYMBOL_SEMICOLON;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
//    int expected = SYMBOL_CCB;
//    if (tokenTag->recognizedState == expected)
//        node->childNodes[childCount++] = getLeaf(expected);
//    else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
//    
//    tokenTag = tokenTag->next;
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pParams() {
//    Node *node = getNode(PARAMS);
//    
//    node->childNodes[0] = pTipo();
//    
//    node->childNodes[1] = getLeaf(ID);
    
    //WARNING: O que fazer em caso de múltiplos PARAMS?
    
//    tokenTag = tokenTag->next;
//    int expected = SYMBOL_COMMA;
//    if (tokenTag->recognizedState == expected) {
//        node->childNodes[2] = getLeaf(expected);
//        
//        tokenTag = tokenTag->next;
//        node->childNodes[3] = pParams();
//    } else printf("Esperado token %s na linha %d, encontrado token %s\n", recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pTipo() {
//    Node *node = getNode(TIPO);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pCmd() {
//    Node *node = getNode(CMD);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pExp() {
//    Node *node = getNode(EXP);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pRexp() {
//    Node *node = getNode(REXP);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pAexp() {
//    Node *node = getNode(AEXP);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pMexp() {
//    Node *node = getNode(MEXP);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pSexp() {
//    Node *node = getNode(SEXP);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pPexp() {
//    Node *node = getNode(PEXP);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

Node *pExps() {
//    Node *node = getNode(EXPS);
    
    Node *node = getNode(EMPTY);
    
    return node;
}

// Utilidades

//Função de comparação de Strings
int strCmp(const char *str1, int str1len, const char *str2){
    int i;
    for(i = 0; i < str1len || str2[i] != '\0'; i++){
        if(str1[i] != str2[i])	return 0;
    }
    if(i < str1len && str2[i] != '\0')	return 0;
    return 1;
}

//Função que remove os espaços vazios de uma String
//char* strtrim(char* s, int len){
//    int i = 0;
//    char *out = (char*) calloc (len + 1, sizeof(char));
//    for(;i < len; i++){
//        out[i] = s[i];
//    }
//    out[i] = '\0';
//    return out;
//}

//Função que remove os espaços vazios de uma String
char *strtrim(char *str) {
    while (isspace(*str)) str++;    //Remove os espaços vazios a esquerda da String

    if (str == 0) return str;   //Retorna a String caso esteja vazia (e.g., a String só possuia espaços vazios)
    
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;   //Remove os espaços vazios a direita da String
    
    *(end+1) = '\0';
    return str;
}