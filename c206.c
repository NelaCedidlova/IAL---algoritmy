/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
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
** uvedeno něco jiného.
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

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
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
void DLL_Init( DLList *list ) {
	list->currentLength = 0;
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */

 //postupne prochazime prvky listu a postupne je uvolnujeme a nakonec inicializujeme jako prazdny list
void DLL_Dispose( DLList *list ) {
	DLLElementPtr el = list->firstElement;
	while(el){
		DLLElementPtr next = el->nextElement;
		free(el);
		el = next;
	}
	
	list->currentLength = 0;
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, long data ) {
	DLLElementPtr new = malloc(sizeof(struct DLLElement));
	if(new == NULL){
		DLL_Error();
		return;
	}

	//pokud je list prazdny, element bude zaroven prvni a posledni a bez sousedu
	if(list->firstElement == NULL){
		list->firstElement = list->lastElement = new;
		new->previousElement = NULL;
		new->nextElement = NULL;
	}else{
		new->nextElement = list->firstElement;	//pokud neni prazdny, nastavime puvodni prvni prvek jako nasledujici prvek noveho 
		list->firstElement->previousElement = new;	//novy element bude predchazejici prvek prvniho
		list->firstElement = new;	//novy prvek je prvnim listu
		new->previousElement = NULL;	//novy prvek nema predchazejici
	}

	new->data = data;	//ulozime data do noveho prvku
	list->currentLength++;	//zvysime pocitadlo poctu prvku listu
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, long data ) {
	DLLElementPtr new = malloc(sizeof(struct DLLElement));
	if(new == NULL){
		DLL_Error();
		return;
	}

	/*pokud je list prazdny, element bude zaroven prvni a posledni a bez sousedu,
	 a jiank to bude obdobne jako u pridavani prvku na zacatek seznamu*/
	if(list->firstElement == NULL){
		list->firstElement = list->lastElement = new;
		new->previousElement = NULL;
		new->nextElement = NULL;
	}else{
		new->nextElement = NULL;
		new->previousElement = list->lastElement;
		list->lastElement->nextElement = new;
		list->lastElement = new;
	}

	new->data = data;
	list->currentLength++;
	
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;	//aktivni prvek bude prvni prvek seznamu
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;	//aktivni prvek bude posledni prvek seznamu
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, long *dataPtr ) {
	if(list->firstElement == NULL){
		DLL_Error();
		return;
	}else{
		*dataPtr = list->firstElement->data;	//predame hodnotu prvniho prvku do pormenne dataPtr
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, long *dataPtr ) {
	if(list->lastElement == NULL){
		DLL_Error();
		return;
	}else{
		*dataPtr = list->lastElement->data;	//predame hodnotu posledniho prvku do pormenne dataPtr
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if(list->firstElement == NULL)
		return;
	DLLElementPtr delF = list->firstElement;	//prvek ktery chceme vymazat

	//pokud byl prvni prvek aktivni, aktivni prvek v listu nebude
	if(delF == list->activeElement)
		list->activeElement = NULL;

	//pokud mel list pouze jeden prvek, list bude potom prazdny
	if(list->firstElement == list->lastElement)
		list->firstElement = list->lastElement = NULL;
	else{
		list->firstElement = delF->nextElement;	//jinak bude novy prvni prvek nasledujici prvek puvodne prvniho
		list->firstElement->previousElement = NULL;
	}

	free(delF);	//uvolnime prvek
	list->currentLength--;	//zmencime pocitadlo prvku v listu
	
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if(list->lastElement == NULL)
		return;

	DLLElementPtr delL = list->lastElement;	//prvek ktery chceme vymazat
	//pokud byl prvni prvek aktivni, aktivni prvek v listu nebude
	if(delL == list->activeElement)
		list->activeElement = NULL;
	
	//pokud mel list pouze jeden prvek, list bude potom prazdny
	if(list->lastElement == list->firstElement)
		list->firstElement = list->lastElement = NULL;
	else{
		list->lastElement = delL->previousElement;	//jinak bude posledni prvek predchozim prvek puvodne posledniho
		list->lastElement->nextElement = NULL;
	}

	free(delL);	//uvolnime prvek
	list->currentLength--;	//snizime pocitadlo prvku
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	//Pokud je seznam list neaktivní nebo pokud je aktivní prvek posledním prvkem seznamu, ukoncime funkci.
	if(list->activeElement == NULL || list->activeElement == list->lastElement)
		return;
		
	DLLElementPtr del = list->activeElement->nextElement;	//prvek ktery chceme vymazat

	//pokud je mazanym prvkem posldni prvek, bude ted aktivnim prvkem prave posledni
	if(del == list->lastElement)
	{
		list->activeElement->nextElement = NULL;
		list->lastElement = list->activeElement;
	}else{
		list->activeElement->nextElement = del->nextElement;	//jinak nasledujici prvek mazaneho prvku bude nasledujicim aktivniho
		del->nextElement->previousElement = list->activeElement;	//aktivni prvek bude predchazejici prvek nasledneho mazaneho prvku
	}
	

	free(del);
	list->currentLength--;
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	//Pokud je seznam list neaktivní nebo pokud je aktivní prvek posledním prvkem seznamu, ukoncime funkci.
	if(list->activeElement == NULL || list->activeElement == list->firstElement)
		return;

	DLLElementPtr del = list->activeElement->previousElement;	//prvek ktery chceme vymazat

	//pokud je mazanym prvkem prvni prvek, bude ted aktivnim prvkem prave prvni
	if(del == list->firstElement){
		list->activeElement->previousElement = NULL;
		list->firstElement = list->activeElement;
	}else {
		list->activeElement->previousElement = del->previousElement; //jinak predchazejici prvek mazaneho prvku bude predchazejicim aktivniho
		del->previousElement->nextElement = list->activeElement; //aktivni prvek bude nadchazejici prvek predchazejiciho mazaneho prvku
	}

	free(del);
	list->currentLength--;
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
void DLL_InsertAfter( DLList *list, long data ) {
	if(list->activeElement == NULL)
		return;

	DLLElementPtr ins = malloc(sizeof(struct DLLElement));
	if(ins == NULL){
		DLL_Error();
		return;
	}

	DLLElementPtr act = list->activeElement;
	DLLElementPtr nxt = list->activeElement->nextElement;	//misto kam chceme novy prvek pridat

	ins->previousElement = act;	//predchazejici prvek vkladaneho bude prave aktivni
	ins->nextElement = nxt;	//nasledujici prvek bude prvek na jehoz misto jsme chteli vkladat

	if(nxt != NULL)
		nxt->previousElement = ins;	//pokud bude prvek za aktivnim existovat, vlozime pred neho novy prvek
	else	
		list->lastElement = ins;	//pokud ne, bude novy prvek poslednim prvkem seznamu

	act->nextElement = ins;
	ins->data = data;
	list->currentLength++;
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
void DLL_InsertBefore( DLList *list, long data ) {
	if(list->activeElement == NULL)
		return;

	DLLElementPtr ins = malloc(sizeof(struct DLLElement));
	if(ins == NULL){
		DLL_Error();
		return;
	}

	DLLElementPtr act = list->activeElement;
	DLLElementPtr prv = list->activeElement->previousElement;	//misto kam chceme novy prvek pridat

	ins->previousElement = prv;	//predchazejici prvek vkladaneho bude prvek na jehoz misto jsme chteli vkladat
	ins->nextElement = act;		//nasledujicim prvkem vkladaneho bude prave aktivni prvek

	if(prv != NULL)
		prv->nextElement = ins;	//pokud bude prvek pred aktivnim existovat, vlozime za neho novy prvek
	else
		list->firstElement = ins;	//pokud ne, bude vkladany prvek prvnim prvek listu

	act->previousElement = ins;
	ins->data = data;
	list->currentLength++;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, long *dataPtr ) {
	if(list->activeElement == NULL){
		DLL_Error();
		return;
	}

	*dataPtr = list->activeElement->data;	//predame do promenne dataPtr data z aktivniho prvku
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, long data ) {
	if(list->activeElement == NULL)
		return;

	list->activeElement->data = data;	//do aktivniho prvku vlozime novou hodnotu
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if(list->activeElement == NULL)
		return;

	list->activeElement = list->activeElement->nextElement; 	//novym aktivnim prvek bude prvek za puvodne aktivnim
	
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if(list->activeElement == NULL)
		return;
	
	list->activeElement = list->activeElement->previousElement;		//novym aktivnim prvkem bude prvek pred puvodne aktivnim
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
bool DLL_IsActive( DLList *list ) {
	return (list->activeElement != NULL);	//pokud aktivni prvek existuje, vrati true(1), jinak false(0)
}

/* Konec c206.c */
