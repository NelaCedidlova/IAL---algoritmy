/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int i = get_hash(key);

  ht_item_t *item = (*table)[i];
  while(item != NULL){
    if(strcmp(key, item->key) == 0)
      return item;
    else
      item = item->next;
  }

  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 * V případě úspěchu funkce tuto skutečnost indikuje návratovou hodnotou 
 * true, v opačném případě (např. při selhání alokace) vrací funkce false.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
bool ht_insert(ht_table_t *table, char *key, float value) {
  int i = get_hash(key);
  ht_item_t *found = ht_search(table, key);
  if(found != NULL){
    found->value = value;
    return true;
  }else{
    ht_item_t *new = malloc(sizeof(ht_item_t));
    if(new == NULL)
      return false;

    new->key = malloc(strlen(key) + 1);
    if(new->key == NULL){
      free(new);
      return false;
    }

    strcpy(new->key, key);
    new->value = value;
    new->next = (*table)[i];
    (*table)[i] = new;

    return true;
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *searched = ht_search(table, key);
  if(searched != NULL){
    return &searched->value;
  }

  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int i = get_hash(key);
  ht_item_t *prev = NULL;
  ht_item_t *curr = (*table)[i];
  while(curr != NULL){
    if(strcmp(key, curr->key) == 0){
      if(prev == NULL){
        (*table)[i] = curr->next;
        free(curr->key);
        free(curr);
        return;
      }else{
        prev->next = curr->next;
        free(curr->key);
        free(curr);
        return;
      }
    }else{
      prev = curr;
      curr = curr->next;
    }

  }
  return;
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  for(int i = 0; i < HT_SIZE; i++){
    ht_item_t *item = (*table)[i];
    if(item != NULL){
      while(item != NULL){
        ht_item_t *next = item->next;
        free(item->key);
        free(item);
        item = next;
      }
      (*table)[i] = NULL;
    }
  }
}
