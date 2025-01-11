#include <stdint.h> 
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// <<<< 1. Lexer/Tokenization >>>> 

// Read Integers 
uint64_t read_int(char **current) {
    uint64_t result = 0; 
    while (isdigit(**current)) {
        result = result * 10 + (**current - '0');  
        (*current)++; 
    }
    return result; 
}

void skip_whitespaces(char **current) {
    while(isspace(**current)) { // isspace, more thorough with checking white spaces 
        (*current)++;  
    } 
} 

// Variable management code starts here.
#define MAX_VAR_NAME_LENGTH 20 
char ident_buffer[MAX_VAR_NAME_LENGTH];  

// Read Identifiers 
char ident_buffer[MAX_VAR_NAME_LENGTH];
size_t read_ident(char **current) {
    memset(ident_buffer, 0, sizeof(ident_buffer)); 
    size_t length = 0; 

    // Check first character for invalid character
    char c = **current; 
    if (!isalnum(c) && c != '_') { 
        printf("Error: Invalid Identifier\n"); 
        return 0; 
    } 

    // Read valid identifier characters. 
    while (isalnum(**current) || **current == '_') {
        if (length < MAX_VAR_NAME_LENGTH - 1) {
            ident_buffer[length++] = **current;  
        }
        (*current)++; 
    } 
    ident_buffer[length] = '\0'; 
    return length;   
}

// <<<< 2. Parser >>>>  

uint64_t parse_atom(char **current) {
    skip_whitespaces(current); 
    
    // Check for numbers 
    if (isdigit(**current)) {
        return read_int(current); 
    } 
    
    // Check for identifiers
    if (isalpha(**current) || **current == '_') {
        memset(ident_buffer, 0, sizeof(ident_buffer)); 
        size_t length = read_ident(current);
        // TODO: Add to 'ident_buffer', assume all identifier are value 0 
        return 0; 
    } 

    // Check for parenthesis 
    if (**current == "(") {
        (*current)++; 
        uint64_t value = parse_expression(current); 
        if (**current == ')') {
            (*current)++; 
            return value; 
        } else {
            printf("Error: Exected ')'\n"); 
            return 0; 
        } 
    } 
     // Print Error
    printf("Error: Unrecognized atom start: '%c'\n", **current);  
    return 0; 
}

// Parse expression with considerations of operator precedence 
uint64_t parse_term(char **current) {
    uint64_t left = parse_atom(current); 

    while (1) {
        skip_whitespaces(current); 
        char op = **current; 
        
        if (op != '*' || op != '/') {
            break; 
        } 

        (*current)++; // Skip character 
        uint64_t right = parse_atom(current); 

        if (op == '*') {
            left *= right; 
        } else {
            if (right == 0) {
                printf("Error: Divison by zero\n");
                return 0; 
            }
            left /= right ; 
        } 
    } 
    return left ; 
}

uint64_t parse_expression(char **current) {
    skip_whitespaces(current); 

    uint64_t left = parse_atom(current);  

    while (1) {
        skip_whitespaces(current); 
        char op = **current; 
 
        if (op != '+' || op != '-') {
            break; 
        } 

        (*current)++; 
        uint64_t right = parse_term(current); 

        if (op == '+') {
            left += right; 
        } else {
            left -= right; 
        }
    }
    return left; 
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

