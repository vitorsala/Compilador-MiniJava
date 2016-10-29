/*
 * main.c
 *
 *  Created on: 24 de ago de 2016
 *      Author: Kawai
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ID_QT 255           //Tamanho maximo de um ID
#define MAX_STR_SIZE 255        //Tamanho maximo de uma String
#define MAX_CHILD_NODES_QT 17 //17 nos filhos, quantidade determinada pelo maior numero de tokens em uma producao (Nesse caso 17 tokens de MAIN)

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

//Representa os tokens reconheciveis
typedef enum {
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
    NUMBEROFSTATES    //Numero total de estados
} RecognizedState;

//Vetor para mapear o numero do enum com a String adequada (e.g., recognizedStates[ID] = "ID>")
char *recognizedStates[] = {
    "ERROR",
    "ID",
    "NUMBER",
    "BOOLEAN",
    "CLASS",
    "EXTENDS",
    "PUBLIC",
    "STATIC",
    "VOID",
    "MAIN",
    "STRING",
    "RETURN",
    "INT",
    "IF",
    "ELSE",
    "WHILE",
    "PRINT",
    "LENGTH",
    "TRUE",
    "FALSE",
    "THIS",
    "NEW",
    "NULL",
    "SYMBOL_OP",
    "SYMBOL_CP",
    "SYMBOL_OB",
    "SYMBOL_CB",
    "SYMBOL_OCB",
    "SYMBOL_CCB",
    "SYMBOL_SEMICOLON",
    "SYMBOL_DOT",
    "SYMBOL_COMMA",
    "OP_EQUAL",
    "OP_MINOR",
    "OP_DOUBLEEQUALS",
    "OP_DIFFERENT",
    "OP_PLUS",
    "OP_MINUS",
    "OP_MULT",
    "OP_AND",
    "OP_NOT"
};

//Representa as producoes (regras) da gramatica
typedef enum {
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
    NT_ID,  //Node Type ID (Ja existe o enumerador ID)
    NUM
} NodeType;

char *nodeTypes[] = {
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

//Estrutura que representa os tokens lidos da lista produzida pela analise lexica
typedef struct TokenTag {
    RecognizedState recognizedState; //Tipo do token
    char value[MAX_STR_SIZE]; //Valor do token em caso de ID ou NUMBER
    int linenum; //Numero da linha do token lido
    struct TokenTag *next; //Estrutura de lista ligada para a analise sintatica
} TokenTag;

//Estrutura de no para a arvore sintatica
typedef struct Node {
    NodeType nodeType; //Tipo do no
    struct Node *childNodes[MAX_CHILD_NODES_QT]; //Nos filhos deste no
    RecognizedState token; //Caso o no seja uma folha, guarda apenas o tipo de token
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
char* rmtrail(char* s, int len);
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

	fprintf(stderr,"Analise lexica finalizada com sucesso");
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
    
    fprintf(stderr,"Analise sintatica finalizada com sucesso");
    getchar();
    
    fclose(input);
    fclose(output);
}


// =======================================================================================
// Analisador Lexico
// =======================================================================================

//Retorna um token com base no id encontrado pelo automato
char* getToken(int id, char* word, int len){
	int i;
	char* attr;
	if(id == ID){
        attr = (char*) calloc (len + 5, sizeof(char));
        char* trimmed = rmtrail(word, len);

		// No caso do ID, e necessario verificar se o ID ja foi utilizado anteriormente
		// para a recuperacao do atributo do token.
		i = 0;
		while(ids[i] != NULL){
			// Para cada ID ja declarado anteriormente, verificar se o ID atual corresponde
			// a algum dos ja declarados
			if(strCmp(trimmed, len, ids[i])){
				sprintf(attr,"ID,%d",i);
				return attr;
			}
			i++;
		}
		ids[i] = trimmed;
		sprintf(attr,"ID,%d",i);
		return attr;
	}
	else if(id == NUMBER){
		// Pequeno tratamento de string para adicionar o valor do digito ao atributo.
        attr = (char*) calloc (len + 9, sizeof(char));
        sprintf(attr, "NUMBER,%s", rmtrail(word, len));
		return attr;
	}
    else if (id > 2 && id < NUMBEROFSTATES) {
        return recognizedStates[id];
    }
	return recognizedStates[ERROR];
}

// Funcao do analisador lexico
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

		// Elimina os espacos em branco antes do texto.
		while(line[i] == ' ' || line[i] == '\t')	i++;

		// Verificacao se nao e um comentario de linha
		if(!(line[i] == '/' && line[i + 1] == '/')){
			while(line[i] != '\0' && line[i] != '\n'){
				// pula os espacos em branco.
				while(line[i] == ' ' || line[i] == '\t')	i++;

				// Comeco do tratamento da cadeia que sera verificado no automato.
				j = 0;
				while(line[i] != ' ' && line[i] != '\0' && line[i] != '\n'){

					if(!isCmt && line[i] == '/' && line[i + 1] == '/'){
						line[i] = '\0';
						break;
					}

					// Verificacao de comentario dentro de uma cadeia de caracteres
					if(!isCmt && line[i] == '/' && line[i + 1] == '*'){
						if(j > 0){
							break;
						}
						else{
							isCmt = 1;
						}
					}
					if(isCmt){	// Se a cadeia estiver apos o inicio de um comentario inline/multi-linha
						if(line[i] == '*' && line[i + 1] == '/'){	// Verifica o final da cadeia de comentario
							isCmt = 0;
							i += 2;
						}
						else	i++;
					}
					// Se nao estiver dentro de um comentario, adicionar o caractere ao buffer
					else 		buffer[j++] = line[i++];
				}
				if(!isCmt && j > 0){
					k = 0;
					bufferIndex = 0;
					buffer[j] = '\0';
					while(buffer[bufferIndex] != '\0' && buffer[bufferIndex] != '\n'){
						// Verificacao de uma palavra.
						int out = automata(buffer + bufferIndex, &k);


						// Dado uma palavra que foi reconhecida, pegar o token correspondente.
						char* str = getToken(out, buffer + bufferIndex, k);
						// output
//						fprintf(stderr,"%s\n", str);
						fprintf(output,"<%s>\r\n", str);

						if(out == ERROR){
							printf("Erro de reconhecimento lexico na linha %d\n", linenum);
                            exit(-1);
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

//Verifica se o character e um simbolo
int issymbol(char c) {
	return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ';' || c == '.' || c == ',';
}

//Verifica se o caracter e um operador
int isop(char c) {
	return c == '=' || c == '<' || c == '!' || c == '+' || c == '-' || c == '*' || c == '&';
}

// Funcao o automato
int automata(char *str, int *index){
	int hasUnderscore = 0;
	int hasDot = 0;

    //Verifica o primeiro caracter da palavra e muda para o estado correspondente, ou retorna ERROR caso o caracter nao seja reconhecido
	*index = 0;
	if(str[*index] == '\0')	return ERROR;
	if(isalpha(str[*index]))	goto qid;   //Vai para o estado id caso o caractere seja uma letra
	if(isdigit(str[*index]))	goto qnum;  //Vai para o estado numeral caso o caracter seja um digito
	if(issymbol(str[*index]))	goto qsym;  //Vai para o estado simbolo caso o caracter seja um simbolo
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
        //Caso um ponto seja reconhecido, guarda na variavel index, que foi passada por referencia, a posicao do ponto
		else if(str[*index] == '.' && hasDot == 0)	hasDot = *index;
		(*index)++;
	}

    //Caso nao tenha underscore...
	if(!hasUnderscore){
        //Caso tenha ponto e a palavra seja equivalente a palavra reservada PRINT, retorna PRINT
		if(hasDot != 0 && strCmp(str, *index, reserved[13])) return RESERVED_PRINT;
		else{
			int i;
            //Caso tenha ponto mas a palavra nao for reconhecida como PRINT, guarda na variavel index, que foi passada por referencia, a posicao do ponto
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
	return ERROR;

//Estado numeral
qnum:
	while(str[*index] != '\0' && isdigit(str[*index]))	(*index)++;
	return NUMBER;
//Estado simbolo
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
		if(str[*index] == '&')	return OP_AND;
	}
	return ERROR;

}

// =======================================================================================
// Analisador Sintatico
// =======================================================================================

//Retorna um enum de token com base na String lida (e.g., "ID" retorna 1)
RecognizedState getRecognizedState(char *token) {
    //i = 1 pula o token Error (Que tem enum 0)
    for (int i = 1; i < NUMBEROFSTATES; i++) {
        if (strcmp(recognizedStates[i], token) == 0)
            return i;
    }
    return ERROR;
}

//Preenche a lista ligada de tokens com base na lista de tokens
void readTokenList(FILE *input) {
    char line[MAX_STR_SIZE];
    int linenum = 0;
    
    while(fgets(line, MAX_STR_SIZE, input) != NULL){
        linenum++;
        tokenTag->linenum = linenum;    //Guarda a linha do token lido
        
        char *token = strtrim(line); //Remove os espacos vazios e cria um ponteiro que aponta para o token lido (Necessario para o metodo strsep)
        
        //O bloco abaixo remove os sinais de '<' e '>' do token
        int toklen = strlen(token); //Tamanho do token
        memmove(&token[0], &token[1], toklen);          //Remove o primeiro caracter da String
        memmove(&token[toklen-2], &token[toklen-1], 1); //Remove o ultimo caracter da String
        
        //Separa o token no delimitador "," (Presente em ID e NUM) e envia a primeira metade para o metodo getRecognizedState, que retorna o tipo do token
        //Caso nao exista o delimitador ",", envia todo o token para o metodo getRecognizedState
        //Atribui o tipo de token (recognizedState) ao Token Tag
        tokenTag->recognizedState = getRecognizedState(strsep(&token, ","));
        if (token != NULL)
            strcpy(tokenTag->value, token); //Atribui o valor do token em Token Tag
        
        //Cria o proximo elemento da lista ligada a ser preenchido pela leitura do arquivo
        tokenTag->next = malloc(sizeof(TokenTag));
        tokenTag = tokenTag->next;
        tokenTag->next = NULL;
        //Obs.: O ultimo elemento da lista ligada sempre sera vazio, importante para evitar segmentation fault
    }
}

//Funcao do analisador sintatico
void parser(FILE *input, FILE *output){
    tokenTag = malloc(sizeof(TokenTag));  //Token Tag guarda as informacoes dos tokens lidos (Variavel global)
    
    TokenTag *head = tokenTag;  //Ponteiro que aponta para o primeiro token da lista
    
    readTokenList(input);
    
    tokenTag = head;
    Node *root = pProg();
    printParseTree(output, root, 0);
    
    //Libera o espaco de memoria ocupado pelos Token Tags
    TokenTag *curr;
    while ((curr = head) != NULL) {
        head = head->next;
        free(curr);
    }
}

// =======================================================================================
// Árvore Sintatica
// =======================================================================================

//Imprime a arvore sintatica no arquivo de saida
void printParseTree(FILE *output, Node *root, int level) {
    if (root->nodeType == LEAF)
        fprintf(output, "%*s" "%d %s\r\n", level * 4, "", level, recognizedStates[root->token]);
    else {
        fprintf(output, "%*s" "%d <%s>\r\n", level * 4, "", level, nodeTypes[root->nodeType]);
        level++;
        for (int i = 0; root->childNodes[i] != NULL; i++)
            printParseTree(output, root->childNodes[i], level);
    }
    
    free(root);
}

//Retorna um no vazio do tipo especificado
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

//Imprime uma mensagem de erro especificando o token esperado, o token lido e a linha relevante do arquivo de tokens
void parseError(char *expected, int linenum, char *found) {
    printf("Esperado token %s na linha %d, encontrado token %s\n", expected, linenum, found);
    exit(-1);
}

/*
 Verifica se o token lido e do tipo esperado.
 Se sim, retorna uma folha com o token especificado.
 Se nao, retorna um no vazio, ou imprime uma menssagem de erro e encerra o programa, dependendo da variavel isSyntaxError.
 A variavel isSyntaxError sera verdadeira para os elementos gramaticais obrigatorios, e falsa para os opcionais.
 */
Node *parse(int expected, bool isSyntaxError) {
    Node *node = NULL;
    if (tokenTag->recognizedState == expected) {
        node = getLeaf(expected);
        tokenTag = tokenTag->next;
    } else if (isSyntaxError)
        parseError(recognizedStates[expected], tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    return node;
}

//Verifica se o proximo token da lista de tokens e do tipo esperado
//Obs.: O lookahead verifica no maximo um token a frente, devido a riscos de segmentation fault
bool lookahead(int expected) {
    return tokenTag->next->recognizedState == expected ? true : false;
}

//Foi feito um metodo diferente para cada producao gramatical
Node *pProg() {
    Node *node = getNode(PROG);
    
    node->childNodes[0] = pMain();
    node->childNodes[1] = pClasse();
    
    return node;
}

Node *pMain() {
    Node *node = getNode(MAIN);
    int childCount = 0;
    
    node->childNodes[childCount++] = parse(RESERVED_CLASS, true);
    node->childNodes[childCount++] = parse(ID, true);
    node->childNodes[childCount++] = parse(SYMBOL_OCB, true);
    node->childNodes[childCount++] = parse(RESERVED_PUBLIC, true);
    node->childNodes[childCount++] = parse(RESERVED_STATIC, true);
    node->childNodes[childCount++] = parse(RESERVED_VOID, true);
    node->childNodes[childCount++] = parse(RESERVED_MAIN, true);
    node->childNodes[childCount++] = parse(SYMBOL_OP, true);
    node->childNodes[childCount++] = parse(RESERVED_STRING, true);
    node->childNodes[childCount++] = parse(SYMBOL_OB, true);
    node->childNodes[childCount++] = parse(SYMBOL_CB, true);
    node->childNodes[childCount++] = parse(ID, true);
    node->childNodes[childCount++] = parse(SYMBOL_CP, true);
    node->childNodes[childCount++] = parse(SYMBOL_OCB, true);
    
    if ((node->childNodes[childCount++] = pCmd()) == NULL)
        childCount--;
    
    node->childNodes[childCount++] = parse(SYMBOL_CCB, true);
    node->childNodes[childCount++] = parse(SYMBOL_CCB, true);
    
    return node;
}

Node *pClasse() {
    if (tokenTag->next == NULL)
        return NULL;
    
    Node *node = getNode(CLASSE);
    int childCount = 0;
    
    node->childNodes[childCount++] = parse(RESERVED_CLASS, true);
    node->childNodes[childCount++] = parse(ID, true);
    
    if ((node->childNodes[childCount++] = parse(RESERVED_EXTENDS, false)) != NULL) {
        node->childNodes[childCount++] = parse(ID, true);
    } else childCount--;
    
    node->childNodes[childCount++] = parse(SYMBOL_OCB, true);
    
    if ((node->childNodes[childCount++] = pVar()) == NULL)
        childCount--;
    
    if ((node->childNodes[childCount++] = pMetodo()) == NULL)
        childCount--;
    
    node->childNodes[childCount++] = parse(SYMBOL_CCB, true);
    node->childNodes[childCount++] = pClasse();
    
    return node;
}

Node *pVar() {
    Node *node = getNode(VAR);
    
    if (lookahead(ID) == false || (node->childNodes[0] = pTipo()) == NULL) {
        free(node);
        return NULL;
    }
    
    node->childNodes[1] = parse(ID, true);
    node->childNodes[2] = parse(SYMBOL_SEMICOLON, true);
    node->childNodes[3] = pVar();
    
    return node;
}

Node *pMetodo() {
    Node *node = getNode(METODO);
    int childCount = 0;
    
    if ((node->childNodes[childCount++] = parse(RESERVED_PUBLIC, false)) == NULL) {
        free(node);
        return NULL;
    }
    
    if ((node->childNodes[childCount++] = pTipo()) == NULL)
        parseError("do tipo TIPO", tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    node->childNodes[childCount++] = parse(ID, true);
    node->childNodes[childCount++] = parse(SYMBOL_OP, true);
    
    if ((node->childNodes[childCount++] = pParams()) == NULL)
        childCount--;
    
    node->childNodes[childCount++] = parse(SYMBOL_CP, true);
    node->childNodes[childCount++] = parse(SYMBOL_OCB, true);
    
    if ((node->childNodes[childCount++] = pVar()) == NULL)
        childCount--;
    
    if ((node->childNodes[childCount++] = pCmd()) == NULL)
        childCount--;
    
    node->childNodes[childCount++] = parse(RESERVED_RETURN, true);
    
    node->childNodes[childCount++] = pExp();
    
    node->childNodes[childCount++] = parse(SYMBOL_SEMICOLON, true);
    node->childNodes[childCount++] = parse(SYMBOL_CCB, true);
    
    return node;
}

Node *pParams() {
    Node *node = getNode(PARAMS);
    
    if ((node->childNodes[0] = pTipo()) == NULL) {
        free(node);
        return NULL;
    }
    
    node->childNodes[1] = parse(ID, true);
    
    if ((node->childNodes[2] = parse(SYMBOL_COMMA, false)) != NULL)
        node->childNodes[3] = pParams();
    
    return node;
}

Node *pTipo() {
    Node *node = getNode(TIPO);
    
    switch (tokenTag->recognizedState) {
        case ID:
            node->childNodes[0] = getLeaf(ID);
            break;
        case RESERVED_BOOLEAN:
            node->childNodes[0] = getLeaf(RESERVED_BOOLEAN);
            break;
        case RESERVED_INT:
            node->childNodes[0] = getLeaf(RESERVED_INT);
            tokenTag = tokenTag->next;
            
            if ((node->childNodes[1] = parse(SYMBOL_OB, false)) != NULL)
                node->childNodes[2] = parse(SYMBOL_CB, true);
            
            return node;    //Return antecipado para evitar avancar tokenTag duas vezes
        default:
            free(node);
            return NULL;
    }
    
    tokenTag = tokenTag->next;
    
    return node;
}

Node *pCmd() {
    Node *node = getNode(CMD);
    
    switch (tokenTag->recognizedState) {
        case SYMBOL_OCB: {
            int childCount = 0;
            
            node->childNodes[childCount++] = getLeaf(SYMBOL_OCB);
            tokenTag = tokenTag->next;
            
            if ((node->childNodes[childCount++] = pCmd()) == NULL)
                childCount--;
            
            node->childNodes[childCount++] = parse(SYMBOL_CCB, true);
            break;
        }
        case RESERVED_IF:
            node->childNodes[0] = getLeaf(RESERVED_IF);
            tokenTag = tokenTag->next;
            node->childNodes[1] = parse(SYMBOL_OP, true);
            node->childNodes[2] = pExp();
            node->childNodes[3] = parse(SYMBOL_CP, true);
            node->childNodes[4] = pCmd();
            
            if ((node->childNodes[5] = parse(RESERVED_ELSE, false)) != NULL)
                node->childNodes[6] = pCmd();
            
            break;
        case RESERVED_WHILE:
            node->childNodes[0] = getLeaf(RESERVED_WHILE);
            tokenTag = tokenTag->next;
            node->childNodes[1] = parse(SYMBOL_OP, true);
            node->childNodes[2] = pExp();
            node->childNodes[3] = parse(SYMBOL_CP, true);
            node->childNodes[4] = pCmd();
            break;
        case RESERVED_PRINT:
            node->childNodes[0] = getLeaf(RESERVED_PRINT);
            tokenTag = tokenTag->next;
            node->childNodes[1] = parse(SYMBOL_OP, true);
            node->childNodes[2] = pExp();
            node->childNodes[3] = parse(SYMBOL_CP, true);
            node->childNodes[4] = parse(SYMBOL_SEMICOLON, true);
            break;
        case ID:
            node->childNodes[0] = getLeaf(ID);
            tokenTag = tokenTag->next;
            
            if ((node->childNodes[1] = parse(OP_EQUAL, false)) != NULL) {
                node->childNodes[2] = pExp();
                node->childNodes[3] = parse(SYMBOL_SEMICOLON, true);
            } else if ((node->childNodes[1] = parse(SYMBOL_OB, false)) != NULL) {
                node->childNodes[2] = pExp();
                node->childNodes[3] = parse(SYMBOL_CB, true);
                node->childNodes[4] = parse(OP_EQUAL, true);
                node->childNodes[5] = pExp();
                node->childNodes[6] = parse(SYMBOL_SEMICOLON, true);
            } else parseError("= ou [", tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
            break;
        default:
            free(node);
            return NULL;
    }
    
    return node;
}

Node *pExp() {
    Node *node = getNode(EXP);
    
    if ((node->childNodes[0] = pRexp()) == NULL)
        parseError("do tipo EXP", tokenTag->linenum, recognizedStates[tokenTag->recognizedState]);
    
    if ((node->childNodes[1] = parse(OP_AND, false)) != NULL)
        node->childNodes[2] = pExp();
    
    return node;
}

Node *pRexp() {
    Node *node = getNode(REXP);
    
    if ((node->childNodes[0] = pAexp()) == NULL) {
        free(node);
        return NULL;
    }
    
    if ((node->childNodes[1] = parse(OP_MINOR, false)) != NULL ||
        (node->childNodes[1] = parse(OP_DOUBLEEQUALS, false)) != NULL ||
        (node->childNodes[1] = parse(OP_DIFFERENT, false)) != NULL)
        node->childNodes[2] = pRexp();
    
    return node;
}

Node *pAexp() {
    Node *node = getNode(AEXP);
    
    if ((node->childNodes[0] = pMexp()) == NULL) {
        free(node);
        return NULL;
    }
    
    if ((node->childNodes[1] = parse(OP_PLUS, false)) != NULL ||
        (node->childNodes[1] = parse(OP_MINUS, false)) != NULL)
        node->childNodes[2] = pAexp();
    
    return node;
}

Node *pMexp() {
    Node *node = getNode(MEXP);
    
    if ((node->childNodes[0] = pSexp()) == NULL) {
        free(node);
        return NULL;
    }
    
    if ((node->childNodes[1] = parse(OP_MULT, false)) != NULL)
        node->childNodes[2] = pMexp();
    
    return node;
}

Node *pSexp() {
    Node *node = getNode(SEXP);
    
    switch (tokenTag->recognizedState) {
        case RESERVED_TRUE:
            node->childNodes[0] = getLeaf(RESERVED_TRUE);
            tokenTag = tokenTag->next;
            break;
        case RESERVED_FALSE:
            node->childNodes[0] = getLeaf(RESERVED_FALSE);
            tokenTag = tokenTag->next;
            break;
        case NUMBER:
            node->childNodes[0] = getLeaf(NUMBER);
            tokenTag = tokenTag->next;
            break;
        case RESERVED_NEW: {
            //Caso seja new ID (E nao, por exemplo, new int)
            if (lookahead(ID))
                goto new_ID;
            
            node->childNodes[0] = getLeaf(RESERVED_NEW);
            tokenTag = tokenTag->next;
            node->childNodes[1] = parse(RESERVED_INT, true);
            node->childNodes[2] = parse(SYMBOL_OB, true);
            node->childNodes[3] = pExp();
            node->childNodes[4] = parse(SYMBOL_CB, true);
            break;
        }
        new_ID:
        default:
            if ((node->childNodes[0] = parse(OP_DIFFERENT, false)) != NULL ||
                (node->childNodes[0] = parse(OP_MINUS, false)) != NULL)
                node->childNodes[1] = pSexp();
            else {
                if ((node->childNodes[0] = pPexp()) == NULL) {
                    free(node);
                    return NULL;
                }
                
                if ((node->childNodes[1] = parse(SYMBOL_DOT, false)) != NULL)
                    node->childNodes[2] = parse(RESERVED_LENGTH, true);
                else if ((node->childNodes[1] = parse(SYMBOL_OB, false)) != NULL) {
                    node->childNodes[2] = pExp();
                    node->childNodes[3] = parse(SYMBOL_CB, true);
                }
            }
            
            break;
    }
    
    return node;
}

Node *pPexp() {
    Node *node = getNode(PEXP);
    int childCount = 0;
    
    switch (tokenTag->recognizedState) {
        case ID:
            node->childNodes[childCount++] = getLeaf(ID);
            tokenTag = tokenTag->next;
            break;
        case RESERVED_THIS:
            node->childNodes[childCount++] = getLeaf(RESERVED_THIS);
            tokenTag = tokenTag->next;
            break;
        case RESERVED_NEW:
            node->childNodes[childCount++] = getLeaf(RESERVED_NEW);
            tokenTag = tokenTag->next;
            node->childNodes[childCount++] = parse(ID, true);
            node->childNodes[childCount++] = parse(SYMBOL_OP, true);
            node->childNodes[childCount++] = parse(SYMBOL_CP, true);
            break;
        case SYMBOL_OP:
            node->childNodes[childCount++] = getLeaf(SYMBOL_OP);
            tokenTag = tokenTag->next;
            node->childNodes[childCount++] = pExp();
            node->childNodes[childCount++] = parse(SYMBOL_CP, true);
            break;
        default:
            free(node);
            return NULL;
    }
    
    if (tokenTag->recognizedState == SYMBOL_DOT && lookahead(ID)) {
        if ((node->childNodes[childCount++] = pPexp()) == NULL)
            childCount--;
        
        node->childNodes[childCount++] = parse(SYMBOL_DOT, true);
        node->childNodes[childCount++] = parse(ID, true);
        
        if ((node->childNodes[childCount++] = parse(SYMBOL_OP, false)) != NULL) {
            node->childNodes[childCount++] = pExps();
            node->childNodes[childCount++] = parse(SYMBOL_CP, true);
        }
    }
    
    return node;
}

Node *pExps() {
    Node *node = getNode(EXPS);
    
    node->childNodes[0] = pExp();
    
    if ((node->childNodes[2] = parse(SYMBOL_COMMA, false)) != NULL)
        node->childNodes[3] = pExp();
    
    return node;
}

// Utilidades

//Funcao de comparacao de Strings
int strCmp(const char *str1, int str1len, const char *str2){
    int i;
    for(i = 0; i < str1len || str2[i] != '\0'; i++){
        if(str1[i] != str2[i])	return 0;
    }
    if(i < str1len && str2[i] != '\0')	return 0;
    return 1;
}

//Funcao que remove o espaco vazio na frente de uma String
char* rmtrail(char* s, int len){
    int i = 0;
    char *out = (char*) calloc (len + 1, sizeof(char));
    for(;i < len; i++){
        out[i] = s[i];
    }
    out[i] = '\0';
    return out;
}

//Funcao que remove os espacos vazios de uma String
char *strtrim(char *str) {
    while (isspace(*str)) str++;    //Remove os espacos vazios a esquerda da String

    if (str == 0) return str;   //Retorna a String caso esteja vazia (e.g., a String so possuia espacos vazios)
    
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;   //Remove os espacos vazios a direita da String
    
    *(end+1) = '\0';
    return str;
}