/** @file
 * Interfejs tekstowy udostępniający operacje na numerach telefonów.
 *
 * @author Zofia Partyka <zp394671@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 01.06.2018
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stddef.h>
#include <stdlib.h>
#include "phone_base.h"
#include "phone_forward.h"
#include "parser_util.h"

/** typedef typu unsigned long long int */
typedef unsigned long long int ulli;

/** @brief Interpretuje alfanumeryczny leksem.
 * Interpretuje alfanumeryczny leksem, ze zwróceniem uwagi na błędy.
 * @param[in, out]  pf         – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in, out]  b          – wskaźnik na początek listy baz;
 * @param[in, out]  c          – znak pobrany z wejścia;
 * @param[in, out]  t          – aktualny token;
 * @param[in, out]  input      – numer znaku z wejścia;
 * @param[in, out]  errorInput – numer znaku z wejścia dla ostatniego znaku operatora.
 * @return            Wskaźnik na początek listy baz.
 */
struct Base *alphanumToken(struct PhoneForward **pf, struct Base *b, int *c, Token *t, ulli *input, ulli *errorInput);

/** @brief Interpretuje liczbowy leksem.
 * Interpretuje liczbowy leksem, ze zwróceniem uwagi na błędy.
 * @param[in]       pf      – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in, out]  b       – wskaźnik na początek listy baz;
 * @param[in, out]  c       – znak pobrany z wejścia;
 * @param[in, out]  num1    – wskaźnik na pierwszy numer;
 * @param[in, out]  num2    – wskaźnik na drugi numer;
 * @param[in, out]  t       – aktualny token;
 * @param[in, out]  input   – numer znaku z wejścia;
 * @param[in, out]  errorInput  – numer znaku z wejścia dla ostatniego znaku operatora.
 * @return            Wskaźnik na strukturę przechowującą przekierowania numerów.
 */
struct PhoneForward *
digitToken(struct PhoneForward *pf, struct Base *b, int *c, char **num1, char **num2, Token *t, ulli *input,
           ulli *errorInput);

/** @brief Interpetuje komentarze.
 * Interpretuje komentarze, ze zwróceniem uwagi na błędy.
 * @param[in]      b     – wskaźnik na początek listy baz;
 * @param[in, out] c     – znak pobrany z wejścia;
 * @param[in, out] num1  – aktualny napis;
 * @param[in, out] input – numer znaku z wejścia.
 */
void commentToken(struct Base *b, int *c, char **num1, ulli *input);

/** @brief Intepretuje operacje '?'.
 * Intepretuje operacje używające operatora '?', ze zwróceniem uwagi na błędy.
 * @param[in, out]  pf         – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in]       b          – wskaźnik na początek listy baz;
 * @param[in, out]  c          – znak pobrany z wejścia;
 * @param[in, out]  num1       – aktualny napis;
 * @param[in, out]  t          – aktualny token;
 * @param[in, out]  input      – numer znaku z wejścia;
 * @param[in, out]  errorInput – numer znaku z wejścia dla ostatniego znaku operatora.
 * @return            Wskaźnik na strukturę przechowującą przekierowania numerów.
 */
struct PhoneForward *
redirToken(struct PhoneForward *pf, struct Base *b, int *c, char **num1, Token *t, ulli *input, ulli *errorInput);

/** @brief Interpretuje operację '>'.
 * Intepretuje operację dodawania przekierowania za pomocą operatora '>'.
 * @param[in, out]  pf         – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in, out]  c          – znak pobrany z wejścia;
 * @param[in, out]  t          – aktualny token;
 * @param[in, out]  input      – numer znaku z wejścia;
 * @param[in, out]  errorInput – numer znaku z wejścia dla ostatniego znaku operatora.
 * @return            Wskaźnik na strukturę przechowującą przekierowania numerów.
 */
struct PhoneForward *addRedirToken(struct PhoneForward *pf, int *c, Token *t, ulli *input, ulli *errorInput);

/**
 * Dzieli leksemy na tokeny, interpretuje je.
 */
void parse(void);

#endif /* __PARSER_H__ */
