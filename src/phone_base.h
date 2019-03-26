/** @file
 * Interfejs klasy obsługującej bazy przekierowań numerów telefonów.
 *
 * @author Zofia Partyka <zp394671@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 01.06.2018
 */

#ifndef __PHONE_BASE_H__
#define __PHONE_BASE_H__

#include <stddef.h>
#include <stdlib.h>
#include "phone_forward.h"

/**
 * Struktura przechowująca unikalne bazy przechowujące przekierowania telefonów.
 */
struct Base {
    char *id; /**< Nazwa bazy przekierowań. */
    struct PhoneForward *pf; /**< Wskaźnik na strukturę przechowująca przekierowania numerów. */
    struct Base *next; /**< Wskaźnik na następny element listy. */
};

/** @brief Tworzy nową bazę.
 * Tworzy nową bazę przekierowań.
 * @return  Wskaźnik na nową bazę lub NULL, gdy nie udało się zaalokować pamięci.
 */
struct Base *baseNew(void);

/** @brief Dodaje nową bazę do listy.
 * Dodaje na początek listy nową bazę przekierowań.
 * @param[in]  id – nazwa bazy;
 * @param[in]  pf – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param  next – wskaźnik na następny element na liście.
 * @return      Wskaźnik na nową początek listy.
 */
struct Base *baseAdd(char *id, struct PhoneForward *pf, struct Base *next);

/** @brief  Usuwa z listy bazę o zadanym @p id.
 * Szuka na liście bazy o takim samym @p id, a następnie usuwa ją.
 * @param[in]  B  – baza do usunięcia;
 * @param[in]  id – nazwa bazy do usunięcia.
 * @return    Wskaźnik na pierwszą bazę na liście.
 */
struct Base *baseRemove(struct Base *B, char *id);

/** Usuwa listę baz.
 * Usuwa listę baz.
 * @param B – początek listy baz.
 */
void deleteList(struct Base *B);

/**
 * Zwalnia pamięć zaalokowaną na potrzeby listy baz.
 * @param[in] B – początek listy baz.
 */
void freeMemory(struct Base *B);

/**
 * Sprawdza, czy lista zawiera daną bazę.
 * @param[in]  b  – szukana baza;
 * @param[in]  id – nazwa szukanej bazy.
 * @return    Wskaźnik na strukturę przechowującą przekierowania numerów.
 */
struct PhoneForward *containsBase(struct Base *b, char *id);

#endif /* __PHONE_BASE_H__ */
