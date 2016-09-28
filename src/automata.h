#ifndef AUTOMATA_H_
#define AUTOMATA_H_

enum recognizedState{
  NONE,
  ID,
  NUMBER,
  RESERVED_BOOL,
  RESERVED_CLASS,
  RESERVED_EXTENDS,
  RESERVED_PUBLIC,
  RESERVED_STATIC,
  RESERVED_VOID,
  RESERVED_MAIN,
  RESERVED_STRING,
  RESERVED_RETURN,
  RESERVED_INT,
  RESERVED_IF,
  RESERVED_ELSE,
  RESERVED_WHILE,
  RESERVED_PRINT, //System.out.println
  RESERVED_LENGTH,
  RESERVED_TRUE,
  RESERVED_FALSE,
  RESERVED_THIS,
  RESERVED_NEW,
  RESERVED_NULL,
  SYMBOL_OP, //Open parenthesis
  SYMBOL_CP, //Close parenthesis
  SYMBOL_OB, //Open bracket
  SYMBOL_CB, //Close bracket
  SYMBOL_OCB, //Open curly bracket
  SYMBOL_CCB, //Close curly bracket
  SYMBOL_SEMICOLON,
  SYMBOL_DOT,
  SYMBOL_COMMA,
  OP_EQUAL, //=
  OP_MINOR, //<
  OP_DOUBLEEQUALS, //==
  OP_DIFFERENT, //!=
  OP_PLUS,
  OP_MINUS,
  OP_MULT,
  OP_AND, //&&
  OP_NOT //!
};

int strCmp(const char *str1, int str1len, const char *str2);
int automata(char *str, int *end);

#endif /* AUTOMATA_H_ */
