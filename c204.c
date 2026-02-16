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

bool solved;

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

/*Prochazim zasobnik dokud nebude prazdny, nactu si operator na vrcholu zasobniku,
pokud je to '(' odstranim ji ze zasobniku a cyklus ukonciim.
Pokud ne, tak ho ulozim do vystupniho pole, popnu a zvysim delku daneho pole*/
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	
	char op;
	while(!Stack_IsEmpty(stack)){
		Stack_Top(stack, &op);
		if(op == '('){
			Stack_Pop(stack);
			break;
		}
		postfixExpression[(*postfixExpressionLength)++] = op;	//do vysledneho pole ukladam operator a zaroven zvysuju pocitadlo jeho delky
		Stack_Pop(stack);
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

//vytvorim si pomocnou funkci pro urceni priority operatoru
static int priority(char operator){
	switch (operator)
	{
	case '+': case '-':
		return 1;
	case '*': case '/':
		return 2;
	default:
		return 0;
	}
}

/*Pouziju moji priority funkci pro urceni priority operatoru v parametru funkce (c) a na vrcholu zasobniku (t).
Pokud je priorita operatoru t vetsi nez priorita operatoru c, vlozim t do vystupniho pole a odstranim z vrcholu
To delam dokud nedojdu na konec zasobniku nebo dokud nenarazim na '(', jakmile se tohle stane, nebo priorita t bude mensi jak c,
pushnu operator c na vrchol zasobniku.*/
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	
	char top;
	int priorC = priority(c);

	while(!Stack_IsEmpty(stack)){
		Stack_Top(stack, &top);
		if(top == '('){
			break;
		}
		int priorTop = priority(top);
		if( priorTop >= priorC){
			postfixExpression[(*postfixExpressionLength)++] = top;	//do vysledneho pole ukladam operator a zaroven zvysuju pocitadlo jeho delky
			Stack_Pop(stack);
		}else{
			break;
		}
	}

	Stack_Push(stack, c);
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
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */

/*Alokuju si a inicializuju stack, naalokuju i pole pro vysledny postfixovy vyraz. Prochazim vstupni pole s infix vyrazem
a podle toho na co narazim, provedu nasledujici operace.*/
char *infix2postfix( const char *infixExpression ) {
	Stack *stack = malloc(sizeof(Stack));
	if(stack == NULL)
		return NULL;

	Stack_Init(stack);

	char* postfixExp = malloc(MAX_LEN*sizeof(char));
	if(postfixExp==NULL){
		Stack_Dispose(stack);
		free(stack);
		return NULL;
	}

	char sign;
	unsigned out = 0;
	for(int i = 0; infixExpression[i] != '\0'; i++){
		sign = infixExpression[i];

		if((sign>='0'&&sign<='9') || (sign>='a'&&sign<='z') || (sign>='A'&&sign<='Z')){
			postfixExp[out++] = sign;	//pokud narazim na cislo nebo pismeno, rovnou ho vlozim do vystupniho pole postfix vyrazu
		}else if(sign == '('){	//pokud narazim na levou zavorku, pushnu ji na stack
			Stack_Push(stack, sign);
		}else if(sign == ')'){	//pokud narazim na pravou zavorku, zavolam funkci untilLeftPar
			untilLeftPar(stack, postfixExp, &out);
		}else{					//pokud narazim na nejaki operator, zavolam funkci doOperation
			doOperation(stack, sign, postfixExp, &out);
		}
	}
	postfixExp[out++] = '=';	//na konec vlozim znak =
	postfixExp[out] = '\0';		//ukoncim retezec

	Stack_Dispose(stack);
	free(stack);
	return postfixExp;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */

/*Cyklus projedeme tolikrat, kolik je velikost intu (v tomto pripade 4x). Postupne ukladame do 1 bytove promenne byty celociselne
4 bytove promenne od LSB po MSB a postupne pushujeme na stack*/
void expr_value_push( Stack *stack, int value ) {
	for(int i = 0; i < (int)sizeof(int); i++){
		char num = (value >> i*8) & 0xFF;
		Stack_Push(stack, num);
	}
	
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */

/*Obdobne jak u expr_value_push iterujeme 4x. Nacteme 1 bytovou hodnotu ze stacku a ukladame ji to 4 bytove hodnoty od MSB po LSB*/
void expr_value_pop( Stack *stack, int *value ) {
	char num;
	int tmp = 0;

	for(int i = 0; i < (int)sizeof(int); i++){
		Stack_Top(stack, &num);
		unsigned char ch = (unsigned char) num;
		tmp |= (ch << ((3-i)*8)) & (0xFF << ((3-i)*8));
		Stack_Pop(stack);
	}
	*value = tmp;
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstupní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @returns true v případě úspěšného vyhodnocení daného výrazu
 * 	 na základě poskytnutých hodnot proměnných, false jinak
 */

 /*vytovrim si pomocnou funkci pro prideleni hodnoty z pole do hodnoty *out*/
static bool getValue(VariableValue arr[], char name, int count, int *out){
	for (int i = 0; i < count; i++)
	{
		//pokud se bude nazev promenne shodovat s tou ulozenou ve strukture, predame jeji hodnotu pres promennou *out
		if(arr[i].name == name){
			*out = arr[i].value;
			return true;
		}
	}
	return false;
}

bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	//naalokujeme si stack a inicializujeme ho
	Stack *stack = malloc(sizeof(Stack));
	if(stack == NULL)
		return false;

	Stack_Init(stack);

	//pomoci funkce infix2postfix prevedeme vstupni vyraz na postfixovy
	char *postfixExpr = infix2postfix(infixExpression);
	
	for(int i = 0; postfixExpr[i] != '='; i++){
		char c = postfixExpr[i];
		//pokud narazime na opernad a pokud jeji hodnota bude ve strukture ulozena, pushneme tuto hodnotu na stack
		if((c>='0'&&c<='9') || (c>='a'&&c<='z') || (c>='A'&&c<='Z')){
			int val;
			if(getValue(variableValues, c, variableValueCount, &val)){
				expr_value_push(stack, val);
			}else return false;
		}else{
			//pokud narazime na operator, popneme ze stacku dve celociselne hodnoty(mame pouze binarni operatory)
			int left;
			int right;
			int result=0;
			expr_value_pop(stack, &right);
			expr_value_pop(stack, &left);
			//podle daneho operatoru provedeme danou operaci
			switch(c){
				case '+': 
					result = left+right;
					break;
				case '-':
					result = left-right;
					break;
				case '*':
					result = left*right;
					break;
				case '/':
					if(right==0) return false;
					result = left/right;
					break;
			}
			expr_value_push(stack, result);	//vysledek pushneme na stack
		}
	}

	expr_value_pop(stack, value);	//vyslednou honotu predame do promenne value
	free(postfixExpr);
	Stack_Dispose(stack);
	free(stack);
	return true;
}

/* Konec c204.c */
