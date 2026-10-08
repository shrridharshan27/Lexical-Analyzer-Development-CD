/**
 * ============================================================================
 * Course: Compiler Design Laboratory (BCSE306L)
 * Experiment 1: Comprehensive Lexical Analyzer Development
 * Author: Shrri Dharshan D R (Reg No: 23BPS1090)
 * Slot: L23+L24
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 500
#define MAX_TOKEN_LEN 256

int current_line = 1;
int current_col = 1;

/* Frequency counters */
int count_keywords = 0;
int count_identifiers = 0;
int count_operators = 0;
int count_literals = 0;
int count_delimiters = 0;
int count_errors = 0;

/* Symbol Table Entry */
typedef struct {
    char name[MAX_TOKEN_LEN];
    int first_line;
} SymbolEntry;

SymbolEntry symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

/* Reserved keywords list */
const char *keywords[] = {
    "int", "float", "char", "double", "void", "if", "else", 
    "while", "for", "do", "return", "var", "end", "break", 
    "continue", "struct", "typedef", "sizeof", NULL
};

int is_keyword(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

void add_to_symbol_table(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) return;
    }
    if (symbol_count < MAX_SYMBOLS) {
        strncpy(symbol_table[symbol_count].name, name, MAX_TOKEN_LEN - 1);
        symbol_table[symbol_count].name[MAX_TOKEN_LEN - 1] = '\0';
        symbol_table[symbol_count].first_line = current_line;
        symbol_count++;
    }
}

int is_delimiter(char c) {
    return (c == '(' || c == ')' || c == '{' || c == '}' || 
            c == '[' || c == ']' || c == ';' || c == ',');
}

int is_operator_char(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || 
            c == '=' || c == '<' || c == '>' || c == '!' || 
            c == '&' || c == '|' || c == '%');
}

void skip_comment(FILE *fp) {
    int next = fgetc(fp);
    if (next == '/') {
        /* Single-line comment: skip until newline */
        int c;
        while ((c = fgetc(fp)) != EOF && c != '\n');
        if (c == '\n') {
            current_line++;
            current_col = 1;
        }
    } else if (next == '*') {
        /* Multi-line comment: skip until '*' '/' */
        int prev = 0, c;
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {
                current_line++;
                current_col = 1;
            } else {
                current_col++;
            }
            if (prev == '*' && c == '/') break;
            prev = c;
        }
    } else {
        ungetc(next, fp);
    }
}

void process_identifier(FILE *fp, int first_char) {
    char buf[MAX_TOKEN_LEN];
    int idx = 0;
    buf[idx++] = (char)first_char;

    int c;
    while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
        if (idx < MAX_TOKEN_LEN - 1) buf[idx++] = (char)c;
        current_col++;
    }
    buf[idx] = '\0';
    if (c != EOF) ungetc(c, fp);

    if (strcmp(buf, "end") == 0) {
        count_keywords++;
        printf("(KEYWORD, end)\n");
        return;
    }

    if (is_keyword(buf)) {
        count_keywords++;
        printf("(KEYWORD, %s)\n", buf);
    } else {
        count_identifiers++;
        add_to_symbol_table(buf);
        printf("(IDENTIFIER, %s)\n", buf);
    }
}

void process_number(FILE *fp, int first_char) {
    char buf[MAX_TOKEN_LEN];
    int idx = 0;
    int has_dot = 0;
    buf[idx++] = (char)first_char;

    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (isdigit(c)) {
            if (idx < MAX_TOKEN_LEN - 1) buf[idx++] = (char)c;
            current_col++;
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            if (idx < MAX_TOKEN_LEN - 1) buf[idx++] = (char)c;
            current_col++;
        } else {
            ungetc(c, fp);
            break;
        }
    }
    buf[idx] = '\0';
    count_literals++;
    if (has_dot) {
        printf("(FLOAT CONSTANT, %s)\n", buf);
    } else {
        printf("(INTEGER CONSTANT, %s)\n", buf);
    }
}

void process_string(FILE *fp) {
    char buf[MAX_TOKEN_LEN * 2];
    int idx = 0;
    buf[idx++] = '"';

    int c;
    while ((c = fgetc(fp)) != EOF) {
        current_col++;
        if (idx < (int)sizeof(buf) - 2) buf[idx++] = (char)c;
        if (c == '"') break;
        if (c == '\n') {
            current_line++;
            current_col = 1;
            count_errors++;
            printf("ERROR: Unterminated string literal at line %d\n", current_line - 1);
            return;
        }
    }
    buf[idx] = '\0';
    count_literals++;
    printf("(STRING, %s)\n", buf);
}

void process_operator(FILE *fp, int first_char) {
    int next = fgetc(fp);
    char op_buf[3];

    /* Compound operators */
    if ((first_char == '=' && next == '=') ||
        (first_char == '!' && next == '=') ||
        (first_char == '<' && next == '=') ||
        (first_char == '>' && next == '=') ||
        (first_char == '&' && next == '&') ||
        (first_char == '|' && next == '|') ||
        (first_char == '+' && next == '+') ||
        (first_char == '-' && next == '-') ||
        (first_char == '+' && next == '=') ||
        (first_char == '-' && next == '=') ||
        (first_char == '*' && next == '=') ||
        (first_char == '/' && next == '=')) {
        op_buf[0] = (char)first_char;
        op_buf[1] = (char)next;
        op_buf[2] = '\0';
        current_col += 2;
    } else {
        if (next != EOF) ungetc(next, fp);
        op_buf[0] = (char)first_char;
        op_buf[1] = '\0';
        current_col++;
    }
    count_operators++;
    printf("(OPERATOR, %s)\n", op_buf);
}

void process_delimiter(char c) {
    count_delimiters++;
    printf("(DELIMITER, %c)\n", c);
    current_col++;
}

void print_summary_and_symbols(void) {
    printf("\n========================================\n");
    printf("             SYMBOL TABLE               \n");
    printf("========================================\n");
    if (symbol_count == 0) {
        printf("  (No user identifiers declared)\n");
    } else {
        for (int i = 0; i < symbol_count; i++) {
            printf("  %-20s | First seen at Line %d\n", symbol_table[i].name, symbol_table[i].first_line);
        }
    }

    printf("\n========================================\n");
    printf("             TOKEN SUMMARY              \n");
    printf("========================================\n");
    printf("  Keywords           : %d\n", count_keywords);
    printf("  Identifiers        : %d\n", count_identifiers);
    printf("  Operators          : %d\n", count_operators);
    printf("  Literals/Constants : %d\n", count_literals);
    printf("  Delimiters         : %d\n", count_delimiters);
    printf("  Lexical Errors     : %d\n", count_errors);
    printf("========================================\n");
}

int run_lexer(FILE *fp) {
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (isspace(c)) {
            if (c == '\n') {
                current_line++;
                current_col = 1;
            } else {
                current_col++;
            }
            continue;
        }

        if (c == '/') {
            int next = fgetc(fp);
            if (next == '/' || next == '*') {
                ungetc(next, fp);
                skip_comment(fp);
                continue;
            }
            if (next != EOF) ungetc(next, fp);
        }

        if (isalpha(c) || c == '_') {
            current_col++;
            process_identifier(fp, c);
            continue;
        }

        if (isdigit(c)) {
            current_col++;
            process_number(fp, c);
            continue;
        }

        if (c == '"') {
            process_string(fp);
            continue;
        }

        if (is_operator_char(c)) {
            process_operator(fp, c);
            continue;
        }

        if (is_delimiter(c)) {
            process_delimiter((char)c);
            continue;
        }

        /* Invalid character */
        count_errors++;
        printf("ERROR: Invalid character '%c' (ASCII %d) at line %d, col %d\n", c, c, current_line, current_col);
        current_col++;
    }

    print_summary_and_symbols();
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *fp = stdin;
    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "Error: Could not open input file '%s'\n", argv[1]);
            return 1;
        }
        printf("Processing file: %s\n\n", argv[1]);
    } else {
        printf("Reading from standard input (Ctrl+Z / Ctrl+D to finish):\n\n");
    }

    int res = run_lexer(fp);
    if (fp != stdin) fclose(fp);
    return res;
}
