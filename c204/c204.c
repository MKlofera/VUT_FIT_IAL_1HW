
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    while (!Stack_IsEmpty(stack)){
        char stackTop;
        Stack_Top(stack, &stackTop);

        Stack_Pop(stack);

        if(stackTop != '('){
            postfixExpression[*postfixExpressionLength + 1] = stackTop;
            *postfixExpressionLength = *postfixExpressionLength + 1;
        }else if (stackTop == '('){
            break;
        }




    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

    while (1){
        //repeat until pushing operator to stack
        char stackTop;
        int stackOperatorPriority = -1;
        int operatorPriority = -1;

        if(stack->topIndex != -1){
            Stack_Top(stack, &stackTop);
            //  what is the operator priority of 'c' argument
            if (c == '+' || c == '-') {
                operatorPriority = 0;
            } else if (c == '*' || c == '/') {
                operatorPriority = 1;
            }

            //  what is the operator priority on top of the stack
            if (stackTop == '+' || stackTop == '-') {
                stackOperatorPriority = 0;
            } else if (stackTop == '*' || stackTop == '/') {
                stackOperatorPriority = 1;
            }
        }

        //check priority of operator on top of stack and in 'c' argument of this function
        if (Stack_IsEmpty(stack) || stackTop == '(' || operatorPriority > stackOperatorPriority) {
            Stack_Push(stack, c);
            break;
        } else if (operatorPriority == stackOperatorPriority || operatorPriority < stackOperatorPriority) {
            Stack_Pop(stack);
            postfixExpression[*postfixExpressionLength + 1] = stackTop;
            *postfixExpressionLength = *postfixExpressionLength + 1;
        }
    }

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    Stack *stack;
    stack = (Stack *) malloc(sizeof(Stack));
    if(stack == NULL){
        return NULL;
    }
    stack->topIndex = -1;

    char *postfix = malloc( sizeof(char) * MAX_LEN );
    if(postfix == NULL){
        free (stack);
        return NULL;
    }

    unsigned postfixLength = -1;

    for (int i = 0; i < MAX_LEN ; i++) {
        char comparedChar = infixExpression[i];
        if (comparedChar == '='){
//            end of infixExpression
            untilLeftPar(stack, postfix, &postfixLength);
            postfix[postfixLength + 1] = '=';
            postfix[postfixLength + 2] = '\0';
            break;

        } else if (comparedChar == '('){
            Stack_Push(stack, comparedChar);
        } else if (
                (comparedChar >= 48 && comparedChar <= 57) ||
                (comparedChar >= 65 && comparedChar <= 90) ||
                (comparedChar >= 97 && comparedChar <= 122)) {
//            comparedChar is a number (0...9) or capital letter (A...Z) or small letter (a...z)
            postfix[postfixLength + 1] = comparedChar;
            postfixLength = postfixLength + 1;
        }else if(
                comparedChar == '+' ||
                comparedChar == '-' ||
                comparedChar == '*' ||
                comparedChar == '/'
                ){

//            comparedChar is a operator
            doOperation(stack, comparedChar, postfix, &postfixLength);
        }else if (comparedChar == ')'){
            untilLeftPar(stack, postfix, &postfixLength);
        }
    }
    free(stack);
    return postfix;
}

/* Konec c204.c */
