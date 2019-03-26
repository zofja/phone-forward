/** @file
 * Interfejs klasy pomocniczej do interfejsu tekstowego.
 *
 * @author Zofia Partyka <zp394671@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 01.06.2018
 */

#ifndef __PARSER_UTIL_H__
#define __PARSER_UTIL_H__

#include <stddef.h>
#include <stdlib.h>
#include "phone_base.h"
#include "phone_forward.h"
#include "parser.h"

/** typedef enumeracji tokenów */
typedef enum {
    EMPTY, ERROR, NUM, NEW, DEL, ADD, GET, REV, REM
} Token;
/** typedef typu unsigned long long int */
typedef unsigned long long int ulli;

/**
 * Struktura przechowująca leksem.
 */
struct Lexeme {
    char *id; /**< Ciąg znaków. */
    int size; /**< Rozmiar leksemu. */
    int pos; /**< Pozycja ostatniego znaku. */
};

/**
 * Tworzy nowy leksem.
 * @return  Wskaźnik na nowy leksem.
 */
struct Lexeme *lexemeNew(void);

/**
 * Dodaje znak do leksemu.
 * @param[in, out] l – wskaźnik na leksem;
 * @param[in] c – znak do dodania do leksemu.
 */
void lexemeAdd(struct Lexeme *l, int c);

/**
 * Pobiera z wejścia liczbę.
 * @param[in, out]  c     – znak pobrany z wejścia;
 * @param[in, out]  input – numer znaku z wejścia.
 * @return       Wskaźnik na leksem liczbowy.
 */
struct Lexeme *numLexemeGet(int *c, ulli *input);

/**
 * Pobiera z wejścia napis.
 * @param[in, out]  c     – znak pobrany z wejścia;
 * @param[in, out]  input – numer znaku z wejścia.
 * @return       Wskaźnik na leksem alfanumeryczny.
 */
struct Lexeme *alphanumLexemeGet(int *c, ulli *input);

/**
 * Pobiera białe znaki z wejścia.
 * @param[in, out]  c     – znak pobrany z wejścia.
 * @param[in, out]  input – numer znaku z wejścia.
 * @return       Znak pobrany z wejścia.
 */
int cleanWhiteCharacters(int c, ulli *input);

/**
 * Inicjanizuje nowy napis.
 * @param[in]  length – długość nowego napisu.
 * @return        Wskaźnik na nowy napis.
 */
char *initNewString(int length);

/** @brief Kopiuje napis.
 * Kopiuje napis długości @p length.
 * @param[in] id – napis do skopiowania;
 * @return    Wskaźnik na nowy napis;
 */
char *copyNewString(char *id);

/** @brief Wypisuje błąd składniowy.
 * Zwalnia pamięć i wypisuje błąd składniowy.
 * @param[in] num   – aktualny napis;
 * @param[in] input – numer znaku z wejścia.
 * @param[in] B     – wskaźnik na początek listy baz.
 */
void inputError(char *num, ulli input, struct Base *B);

/** @brief Wypisuje błąd składniowy.
 * Zwalnia pamięć i wypisuje błąd składniowy. Różni się tym od @p inputError, że jest wywoływana podczas pobierania z wejścia leksemu.
 * @param[in] l     – aktualny leksem;
 * @param[in] input – numer znaku z wejścia.
 * @param[in] B     – wskaźnik na początek listy baz.
 */
void inputLexemError(struct Lexeme *l, ulli input, struct Base *B);

/** @brief Wypisuje błąd składniowy.
 * Zwalnia pamięć i wypisuje błąd składniowy (niespodziewanego zakończenia pliku).
 * @param[in] num – aktualny napis;
 * @param[in] B   – wskaźnik na początek listy baz.
 */
void eofError(char *num, struct Base *B);

/** @brief Wypisuje błąd wykonania.
 * Zwalnia pamięć i wypisuje błąd wykonania.
 * @param[in] l     – aktualny leksem;
 * @param[in] input – numer znaku z wejścia;
 * @param[in] B     – wskaźnik na początek listy baz;
 * @param[in] t     – aktualny token.
 */
void operationError(struct Lexeme *l, ulli input, struct Base *B, Token t);

#endif /* __PARSER_UTIL_H__ */
