/** @file
 * Implementacja klasy pomocniczej do interfejsu tekstowego.
 *
 * @author Zofia Partyka <zp394671@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 01.06.2018
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "phone_base.h"
#include "parser.h"
#include "phone_forward.h"
#include "parser_util.h"

/** Początkowy rozmiar napisu. */
#define INIT_SIZE 3

/** typedef typu unsigned long long int */
typedef unsigned long long int ulli;
/** typedef struktury struct Base */
typedef struct Base Base;
/** typedef struktury struct PhoneForward */
typedef struct PhoneForward PhoneForward;
/** typedef struktury struct Lexeme */
typedef struct Lexeme Lexeme;

Lexeme *lexemeNew(void) {
    Lexeme *lexeme = NULL;
    lexeme = calloc(1, sizeof *lexeme);
    if (lexeme) {
        lexeme->id = NULL;
        lexeme->size = INIT_SIZE;
        lexeme->pos = 0;
    }
    return lexeme;
}

void lexemeAdd(Lexeme *l, int c) {
    if (!l->id) {
        l->id = calloc(INIT_SIZE * 2, sizeof *l->id);
    }
    if (l->id) {
        if (l->pos >= l->size) {
            l->size *= 2;
            l->id = realloc(l->id, (l->size + 1) * sizeof *l->id);
        }
        (l->id)[l->pos] = c;
        (l->id)[l->pos + 1] = '\0';
        (l->pos)++;
    }
}

Lexeme *numLexemeGet(int *C, ulli *input) {
    Lexeme *lexeme = lexemeNew();
    int c = *C;
    while (isdigit(c) && c != EOF) {
        lexemeAdd(lexeme, c);
        c = getchar();
        (*input)++;
    }
    c = cleanWhiteCharacters(c, input);
    *C = c;
    return lexeme;
}

Lexeme *alphanumLexemeGet(int *C, ulli *input) {
    Lexeme *id = lexemeNew();
    int c = *C;
    while ((isalpha(c) || isdigit(c)) && c != EOF) {
        lexemeAdd(id, c);
        c = getchar();
        (*input)++;
    }
    c = cleanWhiteCharacters(c, input);
    *C = c;
    return id;
}

int cleanWhiteCharacters(int c, ulli *input) {
    while (isspace(c)) {
        c = getchar();
        (*input)++;
    }
    return c;
}

char *initNewString(int length) {
    char *string = NULL;
    string = calloc(length + 1, sizeof *string);
    return string;
}

char *copyNewString(char *id) {
    char *num = initNewString(strlen(id));
    strcpy(num, id);
    return num;
}

void inputError(char *num1, ulli input, Base *B) {
    fprintf(stderr, "ERROR %lld\n", input);
    if (num1) {
        free(num1);
    }
    freeMemory(B);
    exit(1);
}

void inputLexemError(Lexeme *l, ulli input, Base *B) {
    fprintf(stderr, "ERROR %lld\n", input);
    free(l->id);
    free(l);
    freeMemory(B);
    exit(1);
}

void eofError(char *num1, Base *B) {
    fprintf(stderr, "ERROR EOF\n");
    if (num1) free(num1);
    freeMemory(B);
    exit(1);
}

void operationError(Lexeme *l, ulli input, Base *B, Token t) {
    switch (t) {
        case DEL:
            fprintf(stderr, "ERROR DEL %lld\n", input);
            break;
        case REV:
            fprintf(stderr, "ERROR ? %lld\n", input);
            break;
        case ADD:
            fprintf(stderr, "ERROR > %lld\n", input);
            break;
        default:
            break;
    }
    free(l->id);
    free(l);
    freeMemory(B);
    exit(1);
}
