/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  if(tree == NULL)
    return false;
  
  while (tree != NULL)
  {
    if(key == tree->key){
      *value = &tree->content;
      return true;
    }else if(key < tree->key){
      tree = tree->left;
    }else{
      tree = tree->right;
    }
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel. V případě úspěchu funkce tuto skutečnost
 * indikuje návratovou hodnotou true, v opačném případě (např. při selhání 
 * alokace) vrací funkce false.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
bool bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  bst_node_t *parent = NULL;
  bst_node_t *curr = *tree;

  while(curr != NULL){
    parent = curr;
    if(key == curr->key){
      free(curr->content.value);
      curr->content = value;
      return true;
    }else if(key < curr->key){
      curr = curr->left;
    }else{
      curr = curr->right;
    }
  }

  bst_node_t *new = malloc(sizeof(bst_node_t));
  if(new == NULL)
    return false;

  new->key = key;
  new->content = value;
  new->left = NULL;
  new->right = NULL;

  if(parent == NULL)  
    *tree = new;
  else if(key < parent->key)
    parent->left = new;
  else
    parent->right = new;
    
  return true;
  
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *parent = NULL;
  bst_node_t *curr = *tree;

  while (curr->right != NULL)
  {
    parent = curr;
    curr = curr->right;
  }

  void *old_val = target->content.value;
  
  target->key = curr->key;
  target->content = curr->content;

  if(parent == NULL){
    *tree = curr->left;
  }else{
    parent->right = curr->left;
  }
  free(old_val);
  free(curr);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  bst_node_t *parent = NULL;
  bst_node_t *curr = *tree;

  while (curr != NULL && curr->key != key) {
    parent = curr;
    curr = (key < curr->key) ? curr->left : curr->right;
  }

  if(curr==NULL)
    return;

  if(curr->left == NULL || curr->right == NULL){
    bst_node_t *child = (curr->left != NULL) ? curr->left : curr->right;

    if(parent == NULL){
        *tree = child;
        }else if(parent->left == curr){
          parent->left = child;
        }else{
          parent->right = child;
        }
        free(curr->content.value);
        free(curr);
        return;
    }

  bst_replace_by_rightmost(curr, &curr->left);

  return;
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if(*tree == NULL)
    return;

  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, *tree);

  while(!stack_bst_empty(&stack)){
    bst_node_t *node = stack_bst_top(&stack);
    node = stack_bst_pop(&stack);
    if(node->left != NULL)
      stack_bst_push(&stack, node->left);
    if(node->right != NULL)
      stack_bst_push(&stack, node->right);

    free(node->content.value);
    free(node);
  }
  *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  while(tree != NULL)
  {
    bst_add_node_to_items(tree, items);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
  
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  while (1)
  {
    bst_leftmost_preorder(tree, &stack, items);
    if(stack_bst_empty(&stack))
      break;
    
    bst_node_t *node = stack_bst_pop(&stack);
    tree = node->right;

  }
  
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  while(1){
    bst_leftmost_inorder(tree, &stack);
    if(stack_bst_empty(&stack))
      break;
    tree = stack_bst_top(&stack);
    bst_add_node_to_items(tree, items); //levy
    tree = stack_bst_pop(&stack);
    tree = tree->right;

  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bool_t visited;
  stack_bool_init(&visited);

  while(1){
    bst_leftmost_postorder(tree, &stack, &visited);

    if(stack_bst_empty(&stack))
      break;

    bst_node_t *node = stack_bst_top(&stack);
    bool first = stack_bool_pop(&visited);
    

    if(first){
      stack_bool_push(&visited, false);
      tree = node->right;
    }else{
      bst_add_node_to_items(node, items);
      (void)stack_bst_pop(&stack);
      tree = NULL;
      
    }
  }
}
