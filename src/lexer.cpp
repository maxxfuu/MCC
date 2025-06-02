#include "../header/lexer.hpp"

bool white_space(char* atom) {
  while (*atom != ' ') {
    atom++;
  }
  return true;
}

// lexer_parse - take raw source code and create a linkedlist of tokens
token_t* lexer_parse(char* source) {

  char* character = source;
  token_t* head;

  while (*character != '\0') {
    if (!white_space(character)) {
    
    }
  }
  return head;
}

/*
 * 1. Input
 * 2. Initialize
 * 3. Scan Loop 
 * 4. Error Handling
 * 5. Handle EOF
 * 6. Output 
 */
