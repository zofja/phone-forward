/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Zofia Partyka <zp394671@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 01.06.2018
 */
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "phone_forward.h"

/** Rozmiar tablicy znaków przechowywanej w węźle drzewa. */
#define ALPHABET_SIZE 10
/** Początkowy rozmiar napisu. */
#define INIT_SIZE 3

/** typedef struktury struct PhoneForward */
typedef struct PhoneForward PhoneForward;
/** typedef struktury struct PhoneNumbers */
typedef struct PhoneNumbers PhoneNumbers;

/**
 * Funkcja porówawcza do qsorta.
 * @param[in]  a – napis;
 * @param[in]  b – napis;
 * @return Różnica między napisami.
 */
static int cmp(const void *a, const void *b) {
    char *p = *(char **) a;
    char *q = *(char **) b;
    return strcmp((char *) p, (char *) q);
}

PhoneForward *phfwdNew(void) {
    PhoneForward *pf = NULL;
    pf = calloc(1, sizeof *pf);
    if (pf) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            pf->children[i] = NULL;
        }
        pf->link = NULL;
    }
    return pf;
}

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę przechowującą numery telefonów.
 * @return  Wskaźnik na strukturę przechowującą numery telefonów.
 */
static PhoneNumbers *phnumNew(void) {
    PhoneNumbers *pnum = NULL;
    pnum = calloc(1, sizeof *pnum);
    pnum->numbers = NULL;
    pnum->quantity = 0;
    return pnum;
}

/**
 * Sprawdza, czy podany napis jest numerem.
 * @param[in]  num – napis;
 * @return Wartość @p true, jeżeli jest numerem.
 *         Wartość @p false w przeciwnym przypadku.
 */
static bool isNumber(char const *num) {
    if (!num) return false;

    for (int i = 0; num[i] != '\0'; i++) {
        if (!isdigit(num[i])) return false;
    }
    return true;
}

/**
 * Inicjanizuje nowy napis.
 * @param[in]  length – długość nowego napisu.
 * @return        Wskaźnik na nowy napis.
 */
static char *initNewString(int length) {
    char *string = NULL;
    string = calloc(length + 1, sizeof *string);
    return string;
}

/**
 * Inicjalizuje nowy numer do struktury przechowującej
 * numery telefonów.
 * @param[in, out]  pnum – wskaźnik na strukturę przechowującą numery telefonów.
 * @param[in]  length – długość inicjalizowanego numeru.
 * @return        Wskaźnik na strukturę przechowującą numery telefonów.
 */
static PhoneNumbers *initNewNumber(PhoneNumbers *pnum, int length) {
    if (!pnum) {
        pnum = phnumNew();
    }

    if (!pnum->numbers) {
        pnum->numbers = calloc(INIT_SIZE, sizeof(*pnum->numbers));
    }
    pnum->numbers[pnum->quantity] = initNewString(length);
    return pnum;
}

/**
 * Dynamicznie alokuje dodatkową pamięć przy pomocy realloca.
 * @param[in, out]  pnum – wskaźnik na strukturę przechowującą ciąg napisów;
 * @return      Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się zaalokować pamięci.
 */
static PhoneNumbers *resize(PhoneNumbers *pnum) {
    pnum->quantity++;
    int newSize = pnum->quantity * 2;
    char **tmp = realloc(pnum->numbers, (newSize) * sizeof(*tmp));
    if (tmp) {
        pnum->numbers = tmp;
    } else return NULL;
    for (int i = pnum->quantity; i < newSize; i++) {
        pnum->numbers[i] = NULL;
    }
    return pnum;
}

/** @brief Kopiuje napis.
 * Dodaje nowy napis do struktury przechowującej numery telefonów poprzez kopiowanie.
 * @param[in, out] pnum   – wskaźnik na strukturę przechowującą numery telefonów;
 * @param[in] string – wskaźnik na napis;
 * @param[in] len    – długość napisu;
 */
static void copyString(PhoneNumbers *pnum, char const *string, int len) {
    pnum = initNewNumber(pnum, len);
    strcpy(pnum->numbers[pnum->quantity], string);
    pnum = resize(pnum);
}

/**
 * Dodaje nowe przekierowanie do trie.
 * @param[in, out]  pf  – trie;
 * @param[in]  num – wskaźnik na napis przekierowywany;
 * @return     Wskaźnik na strukturę przechowującą przekierowania.
 */
static PhoneForward *insert(PhoneForward *pf, char const *num) {

    for (int i = 0; num[i] != '\0'; i++) {
        int index = num[i] - '0';

        if (!pf->children[index]) {
            pf->children[index] = phfwdNew();
        }

        pf = pf->children[index];

    }
    return pf;
}

/**
 * Sprawdza, czy w trie znajduje się napis @p num.
 * @param[in, out]  res – wskaźnik na węzeł, który reprezentuje napis @p num w trie;
 * @param[in]  num – wskaźnik na napis;
 * @return     Wartość @p true, jeżeli napis @p num znajduje się w trie.
 *             Wartość @p false w przeciwnym przypadku.
 */
static bool search(PhoneForward **res, char const *num) {
    int i;
    for (i = 0; num[i] != '\0'; i++) {
        int index = num[i] - '0';

        if (!(*res)->children[index]) {
            return false;
        }
        *res = (*res)->children[index];
    }
    return true;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {

    if (pf) {
        if (num1 && num2) {
            bool added = strcmp(num1, num2) && isNumber(num1) && isNumber(num2);
            if (added) {

                PhoneForward *node = pf;
                if (!search(&node, num1)) {
                    pf = insert(pf, num1);
                    node = pf;
                }

                if (node->link) {
                    free(node->link);
                    node->link = NULL;
                }
                node->link = NULL;
                node->link = initNewString(strlen(num2));
                if (node->link && num2) strcpy(node->link, num2);
            }
            return added;
        }
    }
    return false;
}

/**
 * Rekurencyjnie usuwa z drzewa @p pf dane przekierowanie.
 * @param[in, out] pf – wskaźnik na strukturę przechowującą przekierowania.
 */
void removeRecur(PhoneForward **pf) {
    if (*pf) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if ((*pf)->link) {
                free((*pf)->link);
                (*pf)->link = NULL;
            }
            removeRecur(&(*pf)->children[i]);
        }
    }
}

void phfwdRemove(PhoneForward *pf, char const *num) {

    if (num && isNumber(num)) {
        for (int i = 0; num[i] != '\0'; i++) {
            int index = num[i] - '0';
            if (!pf->children[index]) {
                return;
            }
            pf = pf->children[index];
        }
        removeRecur(&pf);
    }
}

char const *phnumGet(struct PhoneNumbers const *pnum, size_t idx) {

    if (pnum) {
        if (pnum->numbers) {
            if (pnum->numbers[idx]) {
                return pnum->numbers[idx];
            }
        }
    }
    return NULL;
}

/**
 * Dodaje nowy numer do struktury przechowującej numery telefonów.
 * @param[in, out]  pnum  – wskaźnik na strukturę przechowującą numery telefonów;
 * @param[in]  redir – wskaźnik na przekierowanie numeru;
 * @param[in]  num   – wskaźnik na napis;
 * @param[in]  index – wskaźnik na pozycję kończącą przekierowanie;
 * @return       Wskaźnik na strukturę przechowującą numery telefonów.
 */
static PhoneNumbers *getNumber(PhoneNumbers *pnum, char *redir, char const *num, int index) {

    int len = strlen(redir);
    int lenNum = strlen(num);

    pnum = initNewNumber(pnum, len + lenNum + 1);

    memcpy(pnum->numbers[pnum->quantity], redir, len);
    memcpy(pnum->numbers[pnum->quantity] + len, num + index, lenNum - index + 1);

    return pnum;
}

/**
 * Sprawdza, czy w drzewie prefiksów znajduje się przekierowanie danego numeru.
 * @param[in]  pf        – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in]  num       – wskaźnik na napis reprezentujący numer
 * @param[in, out]  resIndex – wskaźnik na indeks cyfry,
 * na którą kończy się najdłuższy pasujący prefiks;
 * @param[in, out]  res       – wskaźnik na węzeł przechowujący dane o najdłuższym pasującym prefiksie
 * @return           Wskaźnik na koniec najdłuższego pasującego prefiksu.
 */
static bool searchForLink(PhoneForward *pf, char const *num, int *resIndex, PhoneForward **res) {
    int i;
    for (i = 0; num[i] != '\0'; i++) {
        int index = num[i] - '0';

        if (!pf->children[index]) {
            return false;
        }
        pf = pf->children[index];

        if (pf->link) {
            *res = pf;
            *resIndex = i;
        }
    }
    if (pf) {
        if (!pf->link) return false;
    }

    return true;
}

PhoneNumbers const *phfwdGet(PhoneForward *pf, char const *num) {

    PhoneForward *res = pf;
    PhoneNumbers *pnum = phnumNew();

    if (isNumber(num)) {
        int resIndex = 0;

        if (searchForLink(pf, num, &resIndex, &res)) {
            copyString(pnum, res->link, strlen(res->link));
        } else {
            if (res->link) {
                pnum = getNumber(pnum, res->link, num, resIndex + 1);
                pnum->quantity++;
            } else {
                copyString(pnum, num, strlen(num));
            }
        }
    }
    return pnum;
}

/** @brief Dodaje znak do wektora.
 * Dodaje znak do wektora.
 * @param key  – wektor;
 * @param c    – znak do dodania;
 * @param pos  – pozycja, na której dodać;
 * @param size – aktualny rozmiar wektora.
 */
static void vectorAdd(char **key, char c, int pos, int *size) {

    if (pos >= *size) {
        *size *= 2;
        char *tmp = realloc(*key, (*size) * sizeof(char **));
        *key = tmp;
    }

    (*key)[pos] = c;
    (*key)[pos + 1] = '\0';
}

/**
 * Sprawdza, czy w węźle znajduje się przekierowanie na prefiks @p num.
 * @param[in]  num   – wskaźnik na napis reprezentujący numer;
 * @param[in]  link  – wskaźnik na napis reprezentujący przekierowanie w danym węźle;
 * @param[in, out]  index – pozycja w @p num, na której kończy prefiks, który jest przekierowywany.
 * @return       Wartość @p true, jeżeli jest przekerowanie na prefiks @p num.
 *               Wartość @p false w przeciwnym przypadku.
 */
static bool isLink(char const *num, char *link, int *index) {

    for (int i = 0; num[i] != '\0' && link[i] != '\0'; i++) {
        if (num[i] != link[i]) return false;
        (*index)++;
    }
    return true;
}

/** @brief Rekurencyjne reverse
 * Rekurencyjnie przechodzi po strukturze @p pf sprawdzając, czy w węźle znajduje się
 * prefiks @p num, tzn. czy węzeł przekierowuje prefiks @p num.
 * @param[in]  pf   – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in]  c    – cyfra reprezentująca dany węzeł;
 * @param[in, out]  key  – wskaźnik na napis reprezentujący aktualną gałąź;
 * @param[in, out]  size – rozmiar napisu reprezentującego aktualną gałąź;
 * @param[in]  num  – wskaźnik na napis reprezentujący numer;
 * @param[in]  lvl  – aktualny poziom w drzewie;
 * @param[in, out]  pnum – wskaźnik na strukturę przechowującą numery telefonów;
 * @return      Wskaźnik na strukturę przechowującą numery telefonów.
 */
static PhoneNumbers *
reverse(PhoneForward *pf, char c, char **key, int *size, char const *num, int lvl, PhoneNumbers *pnum) {

    if (pf) {

        vectorAdd(key, c, lvl, size);

        if (pf->link) {
            if (strlen(pf->link) <= strlen(num)) {
                int index = 0;
                bool ok = isLink(num, pf->link, &index);

                if (ok) {

                    pnum = getNumber(pnum, *key, num, index);
                    pnum = resize(pnum);
                }
            }
        }

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            pnum = reverse(pf->children[i], i + '0', key, size, num, lvl + 1, pnum);
        }
    }
    return pnum;
}

/**
 * Usuwa duplikaty numerów telefonów ze struktury przechowującej wynikowe numery
 * telefonów powstałe w wyniku działania @ref phfwdReverse.
 * @param[in, out]  pnum – wskaźnik na strukturę przechowującą numery telefonów.
 * @return      wskaźnik na strukturę przechowującą numery telefonów bez duplikatów.
 */
static PhoneNumbers *deleteDuplicates(PhoneNumbers *pnum) {

    PhoneNumbers *res = phnumNew();

    bool same = false;
    for (int i = 1; i < pnum->quantity; i++) {
        if (!strcmp(pnum->numbers[i], pnum->numbers[i - 1])) {
            if (!same) {
                for (int j = 0; j < i; j++) {
                    copyString(res, pnum->numbers[j], strlen(pnum->numbers[j]));
                }
                same = true;
            } else {
                copyString(res, pnum->numbers[i - 1], strlen(pnum->numbers[i - 1]));
            }
            while (i < pnum->quantity && !strcmp(pnum->numbers[i], pnum->numbers[i - 1])) {
                i++;
            }
        } else {
            if (same) {
                copyString(res, pnum->numbers[i - 1], strlen(pnum->numbers[i - 1]));
            }
        }
    }
    if (same) {
        phnumDelete(pnum);
        return res;
    } else {
        phnumDelete(res);
        return pnum;
    }
}

PhoneNumbers const *phfwdReverse(PhoneForward *pf, char const *num) {

    PhoneNumbers *pnum = phnumNew();

    if (isNumber(num)) {

        pnum = initNewNumber(pnum, strlen(num));

        for (int i = 0; num[i] != '\0'; i++) {
            pnum->numbers[pnum->quantity][i] = num[i];
        }
        pnum = resize(pnum);
        char *key = NULL;
        key = initNewString(1);
        int size = 1;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (pf->children[i]) {
                char c = i + '0';
                pnum = reverse(pf->children[i], c, &key, &size, num, 0, pnum);
            }
        }
        if (key) {
            free(key);
            key = NULL;
        }
        if (pnum->quantity > 1) {
            qsort(pnum->numbers, pnum->quantity, sizeof(char *), cmp);
            pnum = deleteDuplicates(pnum);
        }
    }
    return pnum;
}

void phfwdDelete(PhoneForward *pf) {
    if (pf) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            phfwdDelete(pf->children[i]);
        }
        if (pf->link) {
            free(pf->link);
            pf->link = NULL;
        }
        if (pf) {
            free(pf);
            pf = NULL;
        }
    }
}
