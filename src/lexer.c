#include "../header/lexer.h"

// Allocate memory for lexer struct
void lexer_create(char* source) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->positon = 0;
    lexer->line = 1;
    lexer->column = 1;
    return lexer;
}

void skip_whitespaces(char* Lexer) {
    
}

