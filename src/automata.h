#ifndef AUTOMATA_H_
#define AUTOMATA_H_

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

#endif /* AUTOMATA_H_ */
