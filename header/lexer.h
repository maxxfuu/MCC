#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <ctype.h>
typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMICOLON,
    TOKEN_EQUALS,
    TOKEN_INT,
    TOKEN_RETURN
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

typedef struct {
    char* source;
    
    int position;
    int line;
    int column;
} Lexer;

void lexer_create(char* source);        // Allocate memory for lexer struct
Token* lexer_parse(char* source);       // Allocate memory for each token
void skip_whitespaces(Lexer* lexer);
void skip_comments(char* Lexer);

#endif
