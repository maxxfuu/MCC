#ifndef LEXER_H
#define LEXER_H

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

Token* lexer_parse(char* source);
void skip_whitespaces(char* Lexer);
Token* create_token(TokenType type, char* value, int line, int column);

#endif
