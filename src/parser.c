/** @file
 * Implementacja tekstowy udostępniający operacje na numerach telefonów.
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
/** typedef struktury struct PhoneNumbers */
typedef struct PhoneNumbers PhoneNumbers;
/** typedef struktury struct Lexeme */
typedef struct Lexeme Lexeme;

Base *alphanumToken(PhoneForward **pf, Base *B, int *C, Token *T, ulli *Input, ulli *ErrorInput) {
    int c = *C;
    Token token = *T;
    ulli input = *Input;

    if (token == EMPTY || token == NEW || token == DEL) {
        if (token == EMPTY) {
            *ErrorInput = input;
            ulli errorInput = input;
            Lexeme *id = alphanumLexemeGet(&c, &input);
            if (!strcmp(id->id, "NEW")) {
                token = NEW;
            } else if (!strcmp(id->id, "DEL")) {
                token = DEL;
            } else {
                inputLexemError(id, errorInput, B);
            }
            free(id->id);
            id->id = NULL;
            free(id);
            id = NULL;
        } else if (token == NEW || token == DEL) {
            ulli errorInput = input;
            Lexeme *id = alphanumLexemeGet(&c, &input);
            if (id->id) {
                if (token == NEW) {
                    if (!strcmp(id->id, "NEW") || !strcmp(id->id, "DEL") || !isalpha(id->id[0])) {
                        inputLexemError(id, errorInput, B);
                    } else {
                        *pf = containsBase(B->next, id->id);
                        if (*pf == NULL) {
                            *pf = phfwdNew();
                            B->next = baseAdd(id->id, *pf, B->next);
                        }
                        token = EMPTY;
                    }
                } else if (token == DEL) {
                    if (!strcmp(id->id, "DEL") || !strcmp(id->id, "NEW") || !isalpha(id->id[0])) {
                        inputLexemError(id, errorInput, B);
                    } else {
                        PhoneForward *tmp = NULL;
                        tmp = containsBase(B->next, id->id);
                        if (!(tmp)) {
                            operationError(id, *ErrorInput, B, token);
                        } else {
                            B = baseRemove(B, id->id);
                            if (*pf == tmp) {
                                *pf = NULL;
                            }
                        }
                        token = EMPTY;
                    }
                }
            }
            free(id->id);
            id->id = NULL;
            free(id);
            id = NULL;
        } else {
            inputError(NULL, input, B);
        }
    } else {
        inputError(NULL, input, B);
    }
    *C = c;
    *T = token;
    *Input = input;
    return B;
}

PhoneForward *
digitToken(PhoneForward *pf, Base *B, int *C, char **Num1, char **Num2, Token *T, ulli *Input, ulli *ErrorInput) {

    int c = *C;
    char *num1 = *Num1;
    char *num2 = *Num2;
    Token token = *T;
    ulli input = *Input;
    ulli errorInput = input;
    Lexeme *num = numLexemeGet(&c, &input);
    if (token == EMPTY) {
        num1 = copyNewString(num->id);
        token = NUM;
    } else if (token == ADD || token == REV || token == DEL) {
        if (token == ADD) {
            num2 = copyNewString(num->id);
            bool add = false;
            if (pf && num1 && num2) {
                add = phfwdAdd(pf, num1, num2);
            }
            if (!add) {
                free(num1);
                free(num2);
                operationError(num, *ErrorInput, B, token);
            }
            free(num2);
            num2 = NULL;
        } else if (token == REV) {
            num1 = copyNewString(num->id);

            if (!pf) {
                free(num1);
                operationError(num, *ErrorInput, B, token);
            }
            PhoneNumbers const *pnum = phfwdReverse(pf, num1);
            for (int i = 0; i < pnum->quantity; i++) {
                fprintf(stdout, "%s\n", pnum->numbers[i]);
            }
            phnumDelete(pnum);
        } else if (token == DEL) {
            if (!pf) {
                free(num1);
                operationError(num, *ErrorInput, B, token);
            }
            num1 = copyNewString(num->id);
            strcpy(num1, num->id);
            phfwdRemove(pf, num1);
        }
        free(num1);
        num1 = NULL;
        token = EMPTY;
    } else {
        if (num1) free(num1);
        if (num2) free(num2);
        inputLexemError(num, errorInput, B);
    }
    if (num) {
        if (num->id) {
            free(num->id);
            num->id = NULL;
        }
        free(num);
        num = NULL;
    }
    *Input = input;
    *C = c;
    *Num1 = num1;
    *Num2 = num2;
    *T = token;
    return pf;
}

void commentToken(Base *B, int *C, char **Num1, ulli *Input) {
    char *num1 = *Num1;
    ulli input = *Input;
    ulli errorInput = input;
    int c = getchar();
    input++;
    if (c == '$') {
        c = getchar();
        input++;
        bool one = false;
        bool two = false;
        while (c != EOF) {
            if (c == '$') {
                if (one) {
                    two = true;
                    break;
                } else {
                    one = true;
                    errorInput = input;
                }
            } else one = false;

            c = getchar();
            input++;
        }
        if (one && !two) {
            if (c == EOF) {
                eofError(num1, B);
            }
            inputError(num1, errorInput, B);
        } else if (!one && !two) {
            eofError(num1, B);
        } else {
            c = getchar();
            input++;
            c = cleanWhiteCharacters(c, &input);
        }
    } else {
        inputError(num1, errorInput, B);
    }
    *C = c;
    *Input = input;
}

PhoneForward *redirToken(PhoneForward *pf, Base *B, int *C, char **Num1, Token *T, ulli *Input, ulli *ErrorInput) {
    Token token = *T;
    int c = *C;
    char *num1 = *Num1;
    ulli input = *Input;
    *ErrorInput = input;
    if (token == NUM) {
        if (!pf) {
            fprintf(stderr, "ERROR ? %lld\n", *ErrorInput);
            free(num1);
            freeMemory(B);
            exit(1);
        }
        PhoneNumbers const *pnum = phfwdGet(pf, num1);
        fprintf(stdout, "%s\n", pnum->numbers[0]);
        phnumDelete(pnum);
        free(num1);
        num1 = NULL;
        token = EMPTY;
    } else if (token == EMPTY) {
        token = REV;
    } else {
        inputError(num1, input, B);
    }
    c = getchar();
    input++;
    c = cleanWhiteCharacters(c, &input);
    *C = c;
    *T = token;
    *Num1 = num1;
    *Input = input;
    return pf;
}

PhoneForward *addRedirToken(PhoneForward *pf, int *C, Token *T, ulli *Input, ulli *ErrorInput) {
    ulli input = *Input;
    Token token = *T;
    int c = *C;
    *ErrorInput = input;
    if (token == NUM) {
        token = ADD;
    } else {
        token = ERROR;
    }
    c = getchar();
    input++;
    c = cleanWhiteCharacters(c, &input);
    *C = c;
    *T = token;
    *Input = input;
    return pf;
}

void parse() {
    Token token = EMPTY;
    PhoneForward *pf = NULL;
    Base *B = baseNew();
    char *num1 = NULL;
    char *num2 = NULL;
    ulli input = 1;
    ulli errorInput = input;
    int c = getchar();
    while (c != EOF) {
        if (isdigit(c)) {
            if (token == NEW) {
                inputError(num1, input, B);
            }
            pf = digitToken(pf, B, &c, &num1, &num2, &token, &input, &errorInput);
        } else if (isalpha(c)) {

            if (token == ADD) {
                inputError(num1, input, B);
            }
            if (token == REV) {
                inputError(num1, input, B);
            }
            if (token == DEL && num1) {
                inputError(num1, input, B);
            }
            if (num1) {
                free(num1);
            }
            B = alphanumToken(&pf, B, &c, &token, &input, &errorInput);
        } else if (c == '$') {

            commentToken(B, &c, &num1, &input);

            c = cleanWhiteCharacters(c, &input);
        } else if (c == '?') {

            pf = redirToken(pf, B, &c, &num1, &token, &input, &errorInput);

        } else if (c == '>') {

            pf = addRedirToken(pf, &c, &token, &input, &errorInput);
            if (token == ERROR) {
                inputError(num1, errorInput, B);
            }
        } else if (c == '@') {


        } else if (isspace(c)) {

            c = cleanWhiteCharacters(c, &input);
        } else {
            inputError(num1, input, B);
        }
        if (c == EOF && token != EMPTY) {
            eofError(num1, B);
        }
    }
    if (num1) free(num1);
    if (num2) free(num2);
    freeMemory(B);
    return;
}
