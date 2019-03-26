/** @file
 * Implementacja klasy obsługującej bazy przekierowań numerów telefonów.
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
#include "phone_forward.h"
#include "phone_base.h"
#include "parser.h"
#include "parser_util.h"

/** typedef struktury struct Base */
typedef struct Base Base;
/** typedef struktury struct PhoneForward */
typedef struct PhoneForward PhoneForward;

Base *baseNew(void) {
    Base *base = NULL;
    base = calloc(1, sizeof *base);
    if (base) {
        base->id = NULL;
        base->pf = NULL;
        base->next = NULL;
    }
    return base;
}

Base *baseAdd(char *Id, PhoneForward *pf, Base *next) {
    Base *b = baseNew();
    char *id = calloc(strlen(Id) + 1, sizeof *id);
    strcpy(id, Id);
    if (b) {
        b->id = id;
        b->pf = pf;
        b->next = next;
    }
    return b;
}

Base *baseRemove(Base *B, char *id) {

    Base *b = B->next;

    if (!strcmp(b->id, id)) {
        B->next = b->next;
        if (b->pf) {
            phfwdDelete(b->pf);
        }
        if (b->id) {
            free(b->id);
        }
        free(b);
        b = NULL;
    } else {
        Base *p = B;

        while (b) {
            if (!strcmp(b->id, id)) {
                Base *tmp = b;
                p->next = b->next;
                b = b->next;
                free(tmp);
                tmp = NULL;
            } else {
                b = b->next;
                p = p->next;
            }
        }
    }
    return B;
}

void deleteList(Base *B) {
    while (B->next) {
        Base *tmp = B;
        if (tmp->pf) {
            phfwdDelete(tmp->pf);
            tmp->pf = NULL;
        }
        if (tmp->id) {
            free(tmp->id);
            tmp->id = NULL;
        }
        B = B->next;
        free(tmp);
        tmp = NULL;
    }
    if (B != NULL) {
        if (B->pf) {
            phfwdDelete(B->pf);
            B->pf = NULL;
        }
        if (B->id) {
            free(B->id);
            B->id = NULL;
        }
        free(B);
        B = NULL;
    }
}

void freeMemory(Base *B) {
    deleteList(B);
}

PhoneForward *containsBase(Base *b, char *id) {
    PhoneForward *pf = NULL;
    while (b) {
        if (!strcmp(id, b->id)) {
            return b->pf;
        }
        b = b->next;
    }
    return pf;
}
