/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je 
 * ukládán průběžně do stromu (klíč vždy lowercase). V případě úspěchu funkce tuto skutečnost
 * indikuje návratovou hodnotou true, v opačném případě (např. při selhání 
 * operace insert) vrací funkce false.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
//funkce pro prevod na lower case a ostatnich znaku na znak '_'

int strlength(char *input){
  int count = 0;
  if(input == NULL)
    return count;

  int i = 0;
  while(input[i] != '\0'){
    count++;
    i++;
  }

  return count;
}


char *convert(char *input){
  int len = strlength(input);
  char *output = malloc((len + 1) * sizeof(char));
  if(output == NULL)
    return NULL;

  for(int i = 0; i < len; i++){
    if(input[i] >= 'A' && input[i] <= 'Z'){
      output[i] = input[i] + 32;
    }else if((input[i] >= 'a' && input[i] <= 'z') || input[i] == ' '){
      output[i] = input[i];
    }
    else{
      output[i] = '_';
    }
  }
  output[len] = '\0';
  return output;
}

bool make_content(int init, bst_node_content_t *out){
  int *val = (int *)malloc(sizeof(int));
  if(val == NULL)
    return false;

  *val = init;
  out->value = val;
  out->type = INTEGER;
  return true;
}

bool letter_count(bst_node_t **tree, char *input) {
  bst_init(tree);
  char *converted = convert(input);
  if(converted == NULL)
    return false;
 
  int len = strlength(converted);
  for(int i = 0; i < len; i++){
    bst_node_content_t *found = NULL;
    bool inserted = bst_search(*tree, converted[i], &found);

    if(inserted && found != NULL){
      if(found->type != INTEGER || found->value == NULL){
        free(converted);
        bst_dispose(tree);
        return false;
      }

      (*(int *)found->value)++;
    }else{
      bst_node_content_t content;
      if(!make_content(1, &content)){
        free(converted);
        bst_dispose(tree);
        return false;
      }

      if(!bst_insert(tree, converted[i], content)){
        free(content.value);
        free(converted);
        bst_dispose(tree);
        return false;
      }
    }
  }
  
  free(converted);
  return true;
}

