// linkedlist.c

#include "linkedlist.h"

Node *ll_insert(Node *head, void *data, Type dt)
{
  head = ll_insert_at_end(head, data, dt);
  return head;
}

static Node *ll_createlist(Node *head, void *data, Type dt)
{
  head=alloc_type(NODE, 1);
  if(!head)
  {
    perror("failure in allocating memory\n");
    free(head);
    return NULL;
  }
  head->data=alloc_type(dt, 1);

  for(int i=0; i<dt_size(dt); i++)
    *(char*)(head->data+i)=*(char*)(data+i);
  head->next=NULL;
  head->size=1;
  return head;
}


Node *ll_search(Node *head, void *data, Type dt)
{
  if(head == NULL)
    return NULL;
  Node *tmp = head;
  while(tmp != NULL)
  {
    if(cmp_cmp(tmp->data, data, dt))
      break;
    tmp = tmp->next;
  }
  return tmp;
}

Node *ll_insert_at_end(Node *head, void *data, Type dt)
{
  if(head==NULL)
  {
    head=ll_createlist(head, data, dt);
    return head;
  }
  Node *newNode=(Node*)alloc_type(NODE, 1);
  newNode->data=alloc_type(dt, 1);

  for(int i=0; i<dt_size(dt); i++)
    *(char*)(newNode->data+i)=*(char*)(data+i);
  newNode->next = NULL;

  Node *tmp=head;
  while(tmp->next!=NULL)
    tmp=tmp->next;

  tmp->next=newNode;
  head->size+=1;
  return head;
}

Node *ll_get_copy(Node *head, Type type)
{
  if(head==NULL) return NULL;
  Node *_tmp=head;
  Node *_new_list=NULL;
  while(_tmp!=NULL)
  {
    _new_list=ll_insert(_new_list, _tmp->data, type);
    _tmp=_tmp->next;
  }
  return _new_list;
}

Node* ll_insert_at_pos( Node *head, int pos, void *data, Type dt) // pos = 2, data = 9
{
  int i=0;
  Node *newNode=alloc_type(NODE, 1);
  newNode->data=alloc_type(dt, 1);

  for (; i<dt_size(dt); i++)
    *(char*)(newNode->data+i)=*(char*)(data+i);
  i=0;

  Node *current=head;

  while(++i!=pos)
    current=current->next;

  newNode->next=current->next;
  current->next=newNode;
#ifdef _LOGGING_
  printf("inserted to linked list at postion %d\n", pos);
#endif
  return head;
}

unsigned int ll_count_of_list(Node *head)
{
  unsigned int count=0;
  if(head==NULL) 
  {
    printf("list is empty\n");
    return 0;
  }
  Node *tmp=head;
  while(tmp!=NULL)
  {
    tmp=tmp->next;
    count++;
  }
  return count;
}

void ll_print_list(Node *head, Type dt)
{
  // check if head it is NULL
  if(head==NULL) 
  {
    printf("LIST IS EMPTY\n"); // the LinkedList is empty.
    return;
  }

  Node *tmp=head; // avoid dealing with LinkedList head directly.
  while(tmp!=NULL)
  {
    switch(dt)
    {
      case CHAR:
        printf("|%c|", *(char*)tmp->data);
        break;
      case INT:
        printf("|%d|", *(int*)tmp->data);
        break;
      case FLOAT:
        printf("|%f|", *(float*)tmp->data);
        break;
      case DOUBLE:
        printf("|%lf|", *(double*)tmp->data);
        break;
      case LONG_DOUBLE:
        printf("|%Lf|", *(long double*)tmp->data);
        break;
      default:
        break;
    }
    if(tmp->next!=NULL)
      printf("-->");
    tmp=tmp->next; // <-- reinitialize tmp to the next Node.
  }
  tmp = NULL;
  return;
}

Node *ll_insert_at_beginning(Node *head, void *data, Type dt)
{
  int i=0;
  Node *newNode=alloc_type(dt, 1);
  newNode->data=alloc_type(dt, 1);
  for (; i<dt_size(dt); i++)
    *(char*)(newNode->data+i)=*(char*)(data+i);
  newNode->next = head;
  head = newNode;

#ifdef _LOGGING_
  printf("INSERTED AT THE BEGINNING OF LIST\n");
#endif
  return head;
}

Node *ll_delete_first_Node( Node *head)
{
  if(head==NULL)
  {
    printf("THE LIST IS EMPTY\n");
    return head;
  }
  if(head->next!=NULL)
    head = head->next;

  Node *tmp=head;
  free(tmp->data);
  tmp->data=NULL;
  free(tmp);
  tmp = NULL;

#ifdef _LOGGING_
  //printf("FIRST NODE DELETED\n");
#endif
  return head;
}


Node *ll_get_last_Node(Node *head)
{
  if(head==NULL)
  {
    printf("list is empty\n");
    return NULL;
  }
  Node *tmp=head;
  while(tmp->next!=NULL)
    tmp=tmp->next;
  return tmp;
}

void ll_delete_last_Node(Node *head)
{
  if(head==NULL)
  {
    printf("list is empty\n");
    return;
  }
  Node *tmp=head;
  while(tmp->next->next!=NULL)
    tmp=tmp->next;

  free(tmp->next->data);
  tmp->next->data=NULL;
  free(tmp->next);
  tmp->next=NULL;
}

Node *ll_delete_at_postion( Node *head, unsigned int pos)
{
  int i=1;
  Node *tmp=head, *tmp2;

  if(pos==1)
  {
    head=ll_delete_first_Node(head);
    return head;
  }
  while(++i!=pos)
    tmp=tmp->next;

  tmp2=tmp->next;
  tmp->next=tmp2->next;

  free(tmp2->data);
  tmp2->data=NULL;
  free(tmp2);
  tmp2=NULL;

  return head;
}

Node *ll_delete_entire_list(Node *head)
{
  if(head==NULL)
  {
    /* printf("list is empty\n"); */
    return head;
  }
  Node *tmp=head;
  while(tmp!=NULL)
  {
    tmp=tmp->next;
    free(head->data);
    head->data=NULL;
    free(head);
    head=NULL;
    head=tmp;
  }
  return head;
}

void ll_inverse_list( Node **head)
{
  Node* prev=NULL;
  Node* current=*head;
  Node* next=NULL; 

  while(current!=NULL) 
  {
    next=current->next;   // Store next Node
    current->next=prev;   // Reverse current Node's pointer
    prev=current;         // Move pointers one position ahead
    current=next;
  }
  *head=prev;   // Update head to point to the new first Node
}


Node *ll_delete_by_node(Node *head, Node *node)
{
  if(head==NULL)
  {
    printf("list is empty\n");
    return head;
  }
  if(head->next==NULL)
  {
    ll_delete_first_Node(node);
    return head;
  }
  Node *tmp=head;
  while(tmp->next != node)
  {
    tmp=tmp->next;
  }

  tmp->next=node->next;
  free(node->data);
  node->data=NULL;
  free(node);
  node->next=NULL;
  node=NULL;
  return head;
}

// ----------------------------------------------------------------------------------------

/* function prototypes */
struct Node* SortedMerge(struct Node* a, struct Node* b);
void FrontBackSplit(struct Node* source,
                    struct Node** frontRef, struct Node** backRef);

/* sorts the linked list by changing next pointers (not data) */
void ll_sort(struct Node** headRef)
{
    struct Node* head=*headRef;
    struct Node* a;
    struct Node* b;

    /* Base case -- length 0 or 1 */
    if ((head==NULL)||(head->next==NULL)) 
      return;

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    ll_sort(&a);
    ll_sort(&b);

    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b);
}

/* See https://www.geeksforgeeks.org/merge-two-sorted-linked-lists/
for details of this function */
struct Node* SortedMerge(struct Node* a, struct Node* b)
{
    struct Node* result = NULL;

    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    /* Pick either a or b, and recur */
    if (*(double*)a->data <= *(double*)b->data) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

void FrontBackSplit(struct Node* source,
                    struct Node** frontRef, struct Node** backRef)
{
    struct Node* fast;
    struct Node* slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}
//---------------------------------------------------------------------------



/* static Node* SortedMerge(struct Node* a, struct Node* b); */
/* static void FrontBackSplit(struct Node* source, Node** frontRef, Node** backRef); */

/* Node *ll_sort(Node* head) */
/* { */
/*   Node* a=NULL; */
/*   Node* b=NULL; */

/*   if((head==NULL)||(head->next==NULL)) */ 
/*     return NULL; */

/*   /1* Split head into 'a' and 'b' sublists *1/ */
/*   FrontBackSplit(head, &a, &b); */

/*   /1* Recursively sort the sublists *1/ */
/*   ll_sort(a); */
/*   ll_sort(b); */

/*   /1* answer = merge the two sorted lists together *1/ */
/*   head = SortedMerge(a, b); */
/*   return head; */
/* } */

/* static Node* SortedMerge(Node* a, Node* b) */
/* { */
/*   Node* result=NULL; */

/*   /1* Base cases *1/ */
/*   if      (a == NULL)   return (b); */
/*   else if (b == NULL)   return (a); */

/*   /1* Pick either a or b, and recur *1/ */
/*   if (*(double*)a->data<*(double*)b->data) */ 
/*   { */
/*     result=a; */
/*     result->next = SortedMerge(a->next, b); */
/*   } */
/*   else */
/*   { */
/*     result = b; */
/*     result->next = SortedMerge(a, b->next); */
/*   } */
/*   return (result); */
/* } */

/* static void FrontBackSplit(Node* source, Node** a, Node** b) */
/* { */
/*   Node* fast=source; */
/*   Node* slow=source->next; */

/*   /1* Advance 'fast' two nodes, and advance 'slow' one node*/ 
/*   while (fast != NULL) */ 
/*   { */
/*     fast = fast->next; */
/*     if (fast != NULL) */ 
/*     { */
/*       slow = slow->next; */
/*       fast = fast->next; */
/*     } */
/*   } */

/*   /1* 'slow' is before the midpoint in the list, so split it in two at that point. *1/ */
/*   *a = source; */
/*   *b = slow->next; */
/*   slow->next = NULL; */
/* } */
