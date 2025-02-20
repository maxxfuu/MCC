#include <stdint.h> 
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h> 
#include <stdlib.h> 

// <<<<< Global Declarations >>>>> 
#define BUFFER_SIZE 100000
uint8_t result_buffer[BUFFER_SIZE]; 
size_t result_size = 0; 
int peak_stack_size = 0;  

#define critical_check_msg(cond, msg) \
    if (!(cond)) { \
        printf(__FILE__ ":%d\n" msg "\nCondition failed: " #cond, __LINE__); \
        exit(1); \
    }

#define critical_check(cond) critical_check_msg(cond, "Compilation aborted !")

typedef uint32_t slot_t; 

#define MAX_VAR_NAME_LENGTH 20
static char ident_buffer[MAX_VAR_NAME_LENGTH];

// Forward Declarations to prevent forward declaratioons 
slot_t slot_allocate(); 
void load_slot_into_x0(slot_t slot); 
void load_slot_into_x1(slot_t slot); 
void push_u32(uint32_t val);
void store_x0_into_slot(slot_t slot);


slot_t add_slots(slot_t a, slot_t b) {
    slot_t result = slot_allocate();
    load_slot_into_x0(a); 
    load_slot_into_x1(b);
    push_u32(0x8B010000); // ARM64 Instruction to ADD X0, X0, X1, storing it in X0 which is X0 
    store_x0_into_slot(result);  
    return result; 
}

slot_t min_slots(slot_t a, slot_t b) {
    slot_t result = slot_allocate();
    load_slot_into_x0(a); 
    load_slot_into_x1(b);
    push_u32(0xCB010000); // ARM64 Instruction to MIN X0, X0, X1, storing it in X0 which is X0 
    store_x0_into_slot(result);  
    return result; 
}

slot_t mul_slots(slot_t a, slot_t b) {
    slot_t result = slot_allocate();
    load_slot_into_x0(a); 
    load_slot_into_x1(b);
    push_u32(0x9B017C00); // ARM64 Instruction to MADD(Multiply) X0, X0, X1, storing it in X0 which is X0 
    store_x0_into_slot(result);  
    return result; 
}

slot_t div_slots(slot_t a, slot_t b) {
    slot_t result = slot_allocate();
    load_slot_into_x0(a); 
    load_slot_into_x1(b);
    push_u32(0x9AC00800); // ARM64 Instruction to UDIV X0, X0, X1, storing it in X0 which is X0 
    store_x0_into_slot(result);  
    return result; 
}

// <<<< 2. Lexer/Tokenization >>>> 
void skip_whitespaces(char **current) {
    while(isspace(**current)) { // isspace, more thorough with checking white spaces 
        (*current)++;  
    } 
}

bool skip_comments(char **current) {
    if (**current == '/') {
        (*current)++; 
        if (**current == '/') { // inline comment 
            while (**current && **current != '\n') 
                (*current)++; 
            critical_check(**current == '\n'); 
            (*current)++; 
            return true;  
        } else if (**current == '*') { // check for mutli-line comment 
            (*current)++; 

            while (**current && !(**current == '/' || **current != '*'))
                (*current)++;

            critical_check_msg(**current != 0, "unterminated comment");
            (*current)++; 
            if (**current == '/') {
                (*current)++; 
                return true; 
            } 
        } else {
            (*current)--; 
        }
    }
    return false; 
}

void skip_gaps(char **current) {
    while (1) {
        skip_whitespaces(current); 
        if (!skip_comments(current))
            break; 
    }
}

uint64_t read_int(char **current) {
    uint64_t result = 0; 
    while (isdigit(**current)) {
        result = result * 10 + (**current - '0');  
        (*current)++; 
    }
    return result; 
}

char read_indent[MAX_VAR_NAME_LENGTH];
size_t read_ident(char **current) {
    memset(read_indent, 0, sizeof(read_indent)); 
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
            read_indent[length++] = **current;  
        }
        (*current)++; 
    } 
    read_indent[length] = '\0'; 
    return length;   
}

// Forward Declaration for Parsing 

slot_t compile_expression(char **current); 
slot_t parse_atom(char **current); 

// <<<<< 3. Parser >>>>> 
slot_t parse_atom(char **current) {
    skip_whitespaces(current); 
    
    // Check for numbers 
    if (isdigit(**current)) { 
        uint64_t val = read_int(current); 
        slot_t s = slot_allocate(); 
        return s; 
    } 
    
    // Check for identifiers
    if (isalpha(**current) || **current == '_') {
        memset(ident_buffer, 0, sizeof(ident_buffer)); 
        size_t length = read_ident(current);
        // TODO: Add to 'ident_buffer', assume all identifier are value 0 
        return 0; 
    } 

    if (**current == '(') { // Check for parenthesis 
        (*current)++; 
        slot_t value = compile_expression(current); 
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

slot_t compile_term(char **current) { // Parse expression with considerations of operator precedence 
    slot_t left = parse_atom(current); 

    while (1) {
        skip_whitespaces(current); 
        char op = **current; 
        
        if (op != '*' && op != '/') {
            break; 
        } 

        (*current)++; // Skip character 
        slot_t right = parse_atom(current); 

        if (op == '*') {
            left = mul_slots(left, right); 
        } else {
            if (right == 0) {
                printf("Error: Divison by zero\n");
                return 0; 
            } 
            left = div_slots(left, right); 
        } 
    } 
    return left ; 
}

slot_t compile_expression(char **current) {
    skip_whitespaces(current); 

    slot_t left = parse_atom(current);  

    while (1) {
        skip_whitespaces(current); 
        char op = **current; 
 
        if (op != '+' && op != '-') {
            break; 
        } 

        (*current)++; 
        slot_t right = parse_atom(current); 

        if (op == '+') {
            left = add_slots(left, right); 
        } else if (op == '-') {
            left = min_slots(left, right); 
        } else if (op == '*') {
            left = mul_slots(left, right); 
        } else {
            left = div_slots(left, right); 
        } 
    }
    return left; 
}

// <<<<< 4. Code Emission >>>>>
slot_t slot_allocate() {
    int32_t result = peak_stack_size;  
    peak_stack_size += 8;
    return result;  
}

// call push whenever I want to emit a singel ARM64 machine instruct of 4 bytes 
void push_u32(uint32_t val) {
    critical_check_msg(result_size + 4 <= BUFFER_SIZE, "Buffer overflow"); 
    result_buffer[result_size++] = (uint8_t)(val & 0xFF); 
    result_buffer[result_size++] = (uint8_t)((val >> 8) & 0xFF); 
    result_buffer[result_size++] = (uint8_t)((val >> 16) & 0xFF); 
    result_buffer[result_size++] = (uint8_t)((val >> 24) & 0xFF); 
}

void load_slot_into_x0(slot_t slot) {
    int32_t offset = slot + 8;
    push_u32(0xF8400000 | ((offset & 0x1FF) << 12) | (29 << 5) | 0);
}

void load_slot_into_x1(slot_t slot) {
    int32_t offset = slot + 8;
    push_u32(0xF8400001 | ((offset & 0x1FF) << 12) | (29 << 5) | 1);
}

void store_x0_into_slot(slot_t slot) {
    int32_t offset = slot + 8;
    push_u32(0xF8000000 | ((offset & 0x1FF) << 12) | (29 << 5) | 0);
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

    { // Block Scope 3, test emission functions. 
        slot_t test_slot = slot_allocate();  // Allocate a slot
        store_x0_into_slot(test_slot);        // This will generate the ARM64 instructions
        
        printf("Generated machine code for storing to slot %u:\n", test_slot);
        for (size_t i = 0; i < result_size; i += 4) {
            printf("Instruction %zu: 0x%02X%02X%02X%02X\n", 
                i/4,
                result_buffer[i+3], 
                result_buffer[i+2], 
                result_buffer[i+1], 
                result_buffer[i]);
        }
    }

    { // Test add_slot()
        result_size = 0;  // reset so we only see newly generated instructions

    slot_t leftAdd = 10;  // Just a mock "slot index"
    slot_t rightAdd = 20; // Another mock "slot index"
    slot_t resultAdd = add_slots(leftAdd, rightAdd); 

    printf("ADD result slot = %u\n", resultAdd);
    for (size_t i = 0; i < result_size; i += 4) {
        printf("Instruction %zu: 0x%02X%02X%02X%02X\n", 
            i/4,
            result_buffer[i+3], 
            result_buffer[i+2], 
            result_buffer[i+1], 
            result_buffer[i]);
        } 
    }
    return 0;

} 

//TODO: load two stack slots into registers, emit an ALU instruction, store back. Specific to ARM64
