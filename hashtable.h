//hashtable.h

#ifndef HASHTABLE_H
#define HASHTABLE_H
#define HT_M 20

#include <stdbool.h>
#include "linkedlist.h"

// some function pointers
unsigned int ht_hash_int_point(void *k, int m);
void ht_print_int(void *n);
void ht_print_float(void *n);
unsigned int ht_hash_set(void *k, int m);
unsigned int ht_hash_int(void *k, int m);
unsigned int ht_hash_float(void *k, int m);
unsigned int ht_hash_double(void *k, int m);

unsigned int ht_hash(void *k, int m, Type dt);
//unsigned int ht_hash(void *k, int m, hash hash_function);
//void *get_hash(Type dt);
//HashTable *ht_create_table(Type dt, hash hash_function);
HashTable *ht_create_table();
HashTable *ht_insert(HashTable *hashtable, void *k, Type dt);
Node *ht_search(HashTable *hashtable, void *k, Type dt);
bool ht_contain(HashTable *hashtable, void *k, Type dt);
HashTable *ht_delete_node(HashTable *hashtable, void *k, Type dt);
void ht_delete(HashTable *hashtable);
static Node *ht_free_node(Node *list, Node* node);
void ht_free_table(HashTable *hashtable);
void ht_print(HashTable *hashtable, Type dt);

#endif
