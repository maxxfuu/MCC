#include <stdint.h> 
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Identifier Buffer 
size_t indentifier_buffer = 0; 

// Tokenixation (Lexing) 
// Token Struct: Pointer to the memory address of a char/string value  
typedef struct {
    char *type; 
    char *value; 
} Token; 

// Read Integers 
uint64_t read_int(char **current) {
    uint64_t result = 0; 
    while (isdigit(**current)) {
        result = result * 10 + (**current - '0');  
        (*current)++; 
    }
    return result; 
}

// Variable management code starts here.
#define MAX_VAR_NAME_LENGTH 20

void skip_whitespaces(char **current) {
    while(isspace(**current)) { // isspace, more thorough with checking white spaces 
        (*current)++;  
    }
    // while (**current == ' ' || **current == '\t' || **current == '\n') {
    //     (*current)++;     
    // } 
} 

// Read Identifiers 
char ident_buffer[MAX_VAR_NAME_LENGTH];
size_t read_ident(char **current) {
    size_t length = 0; 

    // Check if first char is alphanumeric or underscore 
    char c = **current; 
    if (!isalnum(c) && c != '_') {
        return 0; 
    } 

    // Read identifier characters. 
    while (1) {
        char c = **current; 
        if (!isalnum(c) && c != '_') {
            break; 
        }
        (*current)++; 

        ident_buffer[length++] = c;  
    } 
    ident_buffer[length] = '\0'; 
    return length;   
} 

// Test the functions 
int main() {
    { // Block Scope 1   
        char input[] = "123455abc_def"; // poiner to first element in arr 
        char *cursor = input; 
 
        uint64_t number = read_int(&cursor);

        printf("Example 1, test read_int():\n");
        printf("Parsed Integer:%llu\n", number);
    } 

    { // Block Scope 2 
        char input[] = "variable1 is valid.";
        char *cursor = input; 

        printf("Example 2, test read_ident():\n");
        memset(ident_buffer, 0, sizeof(ident_buffer));
        size_t length = read_ident(&cursor); 
        printf("Parsed identifier: %s\n", ident_buffer);
        printf("Identifier length: %zu\n\n", length);

    }

}

// TODO: 1, Finish writing tests for functions. 2, Finish writing Tokenizer/(Lexer)