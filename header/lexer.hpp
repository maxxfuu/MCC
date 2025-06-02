// lexer.hpp
#ifndef LEXER_H
#define LEXER_H

// Token Type
typedef enum {

  // Literals, What a data type can hold
  NUMBER,
  IDENTIFIER,
  STRING,

  // Arithmetic Operator
  PLUS,
  MINUS,
  MULT,
  DIV,
  MOD,

  // Logical Operators
  GREATER,
  LESS,
  GREATER_EQ,
  LESS_EQ,
  NOT_EQ,
  EQUAL,

  // Keywords
  IF,
  ELSE_IF,
  ELSE,
  FOR,
  WHILE,
  RETURN,

  CHAR,
  INT,
  FLOAT,
  DOUBLE,
  VOID,


  // Speical
  COLON,
} token_type_t;

// Token Lexeme - LinkedList
typedef struct token{
  token_type_t tt;
  char* lexeme;
  struct token *next;
} token_t;

token_t* lexer_parse(char* source);
void lexer_push(token_type_t tt, char* lexeme);
bool white_space(char* atom);

#endif
