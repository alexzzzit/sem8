#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[1024];
int pos = 0;

char currentToken[256];

const char *VARIABLE_TYPES[] = {"int", "float", "double", "char", "bool", "void", NULL};
const char *ACCESS_MODIFIERS[] = {"public", "private", "protected", NULL};

void scan();
void match(const char *expected);
void error(const char *message);
void parse_class();
void parse_class_name();
void parse_body();
void parse_access_modifier();
void parse_data_type();

int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_alpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

int is_variable_type(const char *str) {
    for (int i = 0; VARIABLE_TYPES[i]; i++) {
        if (strcmp(str, VARIABLE_TYPES[i]) == 0)
            return 1;
    }
    return 0;
}

int is_access_modifier(const char *str) {
    for (int i = 0; ACCESS_MODIFIERS[i]; i++) {
        if (strcmp(str, ACCESS_MODIFIERS[i]) == 0)
            return 1;
    }
    return 0;
}

// Лексический анализатор
void scan() {
    while (is_space(input[pos])) pos++;

    if (input[pos] == '\0') {
        strcpy(currentToken, "EOF");
        return;
    }

    if (input[pos] == '{' || input[pos] == '}' || input[pos] == ';') {
        currentToken[0] = input[pos++];
        currentToken[1] = '\0';
        return;
    }

    if (is_alpha(input[pos])) {
        int start = pos;
        while (is_alnum(input[pos])) pos++;
        strncpy(currentToken, input + start, pos - start);
        currentToken[pos - start] = '\0';
        return;
    }

    error("Unknown symbol");
}

void match(const char *expected) {
    if (strcmp(currentToken, expected) == 0) {
        scan();
    } else {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected '%s', but got '%s' at position %d", expected, currentToken, pos);
        error(msg);
    }
}

void error(const char *message) {
    printf("Error: %s\n", message);
    exit(1);
}

// Синтаксический анализатор
void parse_class() {
    if (strcmp(currentToken, "class") == 0) {
        match("class");
        parse_class_name();
        match("{");
        parse_body();
        match("}");

        if (strcmp(currentToken, ";") == 0) {
            scan();  
        }
    } else {
        error("Expected 'class' at the beginning");
    }
}

void parse_class_name() {
    if (is_alpha(currentToken[0])) {
        scan();
    } else {
        char msg[256];
        snprintf(msg, sizeof(msg), "Invalid class name '%s' at position %d", currentToken, pos);
        error(msg);
    }
}

void parse_body() {
    while (is_access_modifier(currentToken) || is_variable_type(currentToken) || is_alpha(currentToken[0])) {
        if (is_access_modifier(currentToken)) {
            parse_access_modifier();
        }
        parse_data_type();
        if (is_alpha(currentToken[0])) {
            scan(); // Идентификатор
        } else {
            char msg[256];
            snprintf(msg, sizeof(msg), "Expected identifier, but got '%s' at position %d", currentToken, pos);
            error(msg);
        }
        match(";");
    }
}

void parse_access_modifier() {
    if (is_access_modifier(currentToken)) {
        scan();
    } else {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected access modifier, but got '%s' at position %d", currentToken, pos);
        error(msg);
    }
}

void parse_data_type() {
    if (is_variable_type(currentToken) || is_alpha(currentToken[0])) {
        scan();
    } else {
        char msg[256];
        snprintf(msg, sizeof(msg), "Expected data type, but got '%s' at position %d", currentToken, pos);
        error(msg);
    }
}

int main() {
    printf("Expression from 'class' operator: ");
    fgets(input, sizeof(input), stdin);

    scan();
    parse_class();

    if (strcmp(currentToken, "EOF") != 0) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Unexpected input '%s' at position %d", currentToken, pos);
        error(msg);
    }

    printf("Parsing succeeded!\n");
    return 0;
}

