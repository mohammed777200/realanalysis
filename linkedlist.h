// linkedlist.h
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdio.h>
#include "type.h"
#include "cmp.h"


Node *ll_insert(Node *head, void *data, Type dt);
static Node *ll_createlist(Node *head, void *data, Type dt);
Node *ll_insert_at_pos( Node *head, int pos, void *data, Type dt);
Node *ll_insert_at_beginning( Node *head, void *data, Type dt);
Node *ll_insert_at_end( Node *ptr, void *data, Type dt);
Node *ll_get_last_Node(Node *head);
Node *ll_search(Node *const head, void *data, Type dt);
Node *ll_get_copy(Node *head, Type type);
/* Node *ll_sort(Node *head); */
void ll_sort(struct Node** headRef);

unsigned int ll_count_of_list( Node *head);
void ll_print_list(Node *head, Type dt);
void ll_inverse_list( Node **head);

Node *ll_delete_first_Node( Node *head);
void ll_delete_last_Node( Node *head);
Node *ll_delete_at_postion( Node *head, unsigned int pos);
Node *ll_delete_entire_list( Node *head);
Node *ll_delete_by_node(Node *head, Node *node);
#endif
