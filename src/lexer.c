#include "../header/lexer.h"

// Allocate memory for lexer struct
void lexer_create(char* source) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

void skip_whitespaces(Lexer* lexer) {
    while(isspace(lexer->source[lexer->position])) {
        if (lexer->source[lexer->position] == '\n') {
            lexer->line++; // increment
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

