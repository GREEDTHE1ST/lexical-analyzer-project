#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int nextToken;
FILE *in_fp;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes from the example output */
#define INT_LIT 10
#define IDENT 11
#define ADD_OP 21
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define EOF_TOKEN -1

/**
 * main - The main driver of the program.
 * It opens the input file and calls the lexical analyzer
 * until the end of the file is reached.
 */
int main() {
    /* Open the input file for reading */
    if ((in_fp = fopen("expression.txt", "r")) == NULL) {
        printf("ERROR - cannot open expression.txt\n");
    } else {
        getChar(); // Get the first character
        do {
            lex(); // Analyze the next lexeme and token
        } while (nextToken != EOF_TOKEN);
    }
    return 0;
}

/**
 * lookup - A function to identify single-character tokens like operators
 * and parentheses.
 * @ch: The character to look up.
 * Returns the corresponding token code.
 */
int lookup(char ch) {
    switch (ch) {
        case '(': addChar(); nextToken = LEFT_PAREN; break;
        case ')': addChar(); nextToken = RIGHT_PAREN; break;
        case '+': addChar(); nextToken = ADD_OP; break;
        case '/': addChar(); nextToken = DIV_OP; break;
        default:  addChar(); nextToken = EOF_TOKEN; strcpy(lexeme, "EOF"); break;
    }
    return nextToken;
}

/**
 * addChar - A helper function to append the next character to the lexeme string.
 */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0'; // Null-terminate the string
    } else {
        printf("Error - lexeme is too long\n");
    }
}

/**
 * getChar - A helper function to get the next character from the input file
 * and determine its class (LETTER, DIGIT, or UNKNOWN).
 */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/**
 * getNonBlank - A utility to skip over any whitespace characters.
 */
void getNonBlank() {
    while (isspace(nextChar)) {
        getChar();
    }
}

/**
 * lex - The core lexical analyzer.
 * This function reads characters to form a complete lexeme and
 * determines its token type.
 * Returns the integer code for the token.
 */
int lex() {
    lexLen = 0;
    getNonBlank(); // Skip whitespace

    switch (charClass) {
        /* Case 1: Parse identifiers (like 'sum' or 'total') */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;

        /* Case 2: Parse integer literals (like '47' or '96') */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Case 3: Parse parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* Case 4: End of File */
        case EOF:
            nextToken = EOF_TOKEN;
            strcpy(lexeme, "EOF");
            break;
    }

    // Print the result for the identified token
    printf("Next token is: %d Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}
