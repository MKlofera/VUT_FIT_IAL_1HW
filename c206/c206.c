
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
    printf("*ERROR* The program has performed an illegal operation.\n");
    error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init(DLList *list) {
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose(DLList *list) {
    if(list->firstElement){
        while (list->firstElement != NULL) {
            if (list->lastElement != NULL) {
                if (list->lastElement == list->activeElement) {
                    list->activeElement = NULL;
                }
                struct DLLElement *toDelete = list->lastElement;
                if (list->lastElement->previousElement) {
//      more than one element
                    list->lastElement = list->lastElement->previousElement;
                    list->lastElement->nextElement = NULL;
                } else {
//      deleting last element
                    list->lastElement = NULL;
                    list->firstElement = NULL;
                }
                free(toDelete);
            }
        }
        // deleting the first one left
        free(list->firstElement);
        list->activeElement = NULL;
        list->firstElement = NULL;
        list->lastElement = NULL;
    }
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst(DLList *list, int data) {
    struct DLLElement *newElement = (struct DLLElement *) malloc(sizeof(struct DLLElement));
    if (newElement == NULL) {
        DLL_Error();
    } else {
        newElement->data = data;
        newElement->previousElement = NULL;
        if (list->firstElement == NULL) {
//          adding first element to list
            list->firstElement = newElement;
            list->lastElement = newElement;
        } else {
//          adding another element to list
            newElement->nextElement = list->firstElement;
            list->firstElement->previousElement = newElement;
            list->firstElement = newElement;
        }
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast(DLList *list, int data) {
    struct DLLElement *newElement = (struct DLLElement *) malloc(sizeof(struct DLLElement));
    if (newElement == NULL) {
        DLL_Error();
    } else {
        newElement->data = data;
        newElement->nextElement = NULL;
        if (list->firstElement == NULL) {
//          adding first element to list
            newElement->previousElement = NULL;
            list->firstElement = newElement;
            list->lastElement = newElement;
        } else {
            newElement->previousElement = list->lastElement; // NULL or pointer
            list->lastElement->nextElement = newElement;
            list->lastElement = newElement;
        }
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First(DLList *list) {
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last(DLList *list) {

    list->activeElement = list->lastElement;

}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst(DLList *list, int *dataPtr) {

    if (list->firstElement == NULL) {
        DLL_Error();
    } else {
        *dataPtr = list->firstElement->data;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast(DLList *list, int *dataPtr) {

    if (list->lastElement == NULL) {
        DLL_Error();
    } else {
        *dataPtr = list->lastElement->data;
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst(DLList *list) {
    if (list->firstElement != NULL) {
        if (list->firstElement == list->activeElement) {
            list->activeElement = NULL;
        }
        struct DLLElement *toDelete = list->firstElement;
        if(list->firstElement->nextElement){
//      more than one element
            list->firstElement = list->firstElement->nextElement;
            list->firstElement->previousElement = NULL;
        }else{
//      deleting last element
            list->firstElement = NULL;
            list->lastElement = NULL;
        }
        free(toDelete);
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast(DLList *list) {
    if (list->lastElement != NULL) {
        if (list->lastElement == list->activeElement) {
            list->activeElement = NULL;
        }
        struct DLLElement *toDelete = list->lastElement;
        if (list->lastElement->previousElement) {
//      more than one element
            list->lastElement = list->lastElement->previousElement;
            list->lastElement->nextElement = NULL;
        } else {
//      deleting last element
            list->lastElement = NULL;
            list->firstElement = NULL;
        }
        free(toDelete);
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter(DLList *list) {

    if(list->activeElement && list->activeElement->nextElement){
        struct DLLElement *toDelete = list->activeElement->nextElement;
        if(toDelete->nextElement){
//      after deleting element is another element
            list->activeElement->nextElement = toDelete->nextElement;
            toDelete->nextElement->previousElement = list->activeElement;
        }else if(!toDelete->nextElement){
//      after deleting element is NULL
            list->activeElement->nextElement = NULL;
            list->lastElement = list->activeElement;
        }
        free(toDelete);
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore(DLList *list) {
    if(list->activeElement && list->activeElement->previousElement){
        struct DLLElement *toDelete = list->activeElement->previousElement;
        if(toDelete->previousElement){
//      before deleting element is another element
            list->activeElement->previousElement = toDelete->previousElement;
            toDelete->previousElement->nextElement = list->activeElement;
        }else{
//      after deleting element is NULL
            list->activeElement->previousElement = NULL;
            list->firstElement = list->activeElement;
        }
        free(toDelete);
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter(DLList *list, int data) {

    if (list->activeElement){
        struct DLLElement *newElement = (struct DLLElement *) malloc(sizeof(struct DLLElement));
        if (newElement == NULL) {
            DLL_Error();
        } else {
            newElement->data = data;
            newElement->previousElement = list->activeElement;
            if(list->activeElement->nextElement){
//          active element is NOT last element in the list
                newElement->nextElement = list->activeElement->nextElement;
                struct DLLElement *tmp = list->activeElement->nextElement;
                tmp->previousElement = newElement;
            }else{
                newElement->previousElement = NULL;
            }
            list->activeElement->nextElement = newElement;
            if(list->activeElement == list->lastElement){
                list->lastElement = list->activeElement->nextElement;
            }
        }
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore(DLList *list, int data) {
    if (list->activeElement){
        struct DLLElement *newElement = (struct DLLElement *) malloc(sizeof(struct DLLElement));
        if (newElement == NULL) {
            DLL_Error();
        } else {
            newElement->data = data;
            newElement->nextElement = list->activeElement;
            if(list->activeElement->previousElement){
//          active element is NOT first element
                newElement->previousElement = list->activeElement->previousElement;
                struct DLLElement *tmp = list->activeElement->previousElement;
                tmp->nextElement = newElement;
            }else{
                newElement->previousElement = NULL;
            }
            list->activeElement->previousElement = newElement;
            if(list->activeElement == list->firstElement){
                list->firstElement = list->activeElement->previousElement;
            }
        }
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue(DLList *list, int *dataPtr) {

    if (list->activeElement) {
        *dataPtr = list->activeElement->data;
    } else {
        DLL_Error();
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue(DLList *list, int data) {

    if (list->activeElement) {
        list->activeElement->data = data;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next(DLList *list) {

    if (list->activeElement) {
        struct DLLElement *tmp = list->activeElement;
        list->activeElement = tmp->nextElement;
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous(DLList *list) {

    if (list->activeElement) {
        list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive(DLList *list) {

    return list->activeElement ? 1 : 0;
}

/* Konec c206.c */
