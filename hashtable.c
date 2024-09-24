//hashtable.c

#include "hashtable.h"

#define HASHING(type, k, m)\
  type _k = *(type*)k;\
  unsigned int n = (unsigned int)_k;\
  if(n<0)\
    n=n*-1;\
  _hash=n % m;\


inline unsigned int ht_hash(void *k, int m, Type dt)
{
  unsigned int _hash=0;\
  switch(dt)
  {
    case INT:
      {
        HASHING(int, k, m)
      }
      break;
    case FLOAT:
      {
        HASHING(float, k, m)
      }
      break;
    case DOUBLE:
      {
        HASHING(double, k, m)
      }
      break;
    case LONG_DOUBLE:
      {
        HASHING(long double, k, m)
      }
      break;
    case CHAR:
      {
        HASHING(char, k, m)
      }
      break;
    case SET:
      {
        Set s=*(Set*)k;
        int i = s.cardinal;
        if(i<0)
          i=i*-1;
        _hash=i%m;
      }
    case POINT:
      {
        Point p=*(Point*)k;
        int x = *(int*)p.x;
        int y = *(int*)p.y;
        int r = x+y;

        if(r<0)
          r=r*-1;
        _hash=r%m;
      }
    default:
      break;
  }
  return _hash;
}

HashTable *ht_create_table()
{
#ifdef _LOGGING_
  //printf("creating table....\n");
#endif
  Node **table = (Node**)calloc(HT_M, sizeof(Node*));
  // copying data to hashtable data
  if(!table)
  {
    perror("failure in allocate memory for table\n");
    return NULL;
  }

  HashTable *hashtable = (HashTable*)malloc(sizeof(HashTable));
  if(!hashtable)
  {
    perror("failed to allocate memory for hashtable\n");
    free(table);
    return NULL;
  }
  hashtable->table = table;
  // allocating table items memory
  hashtable->table_count = HT_M;
  //hashtable->hash_function=hash_function;
  hashtable->sorts = NULL;
  return hashtable;
}

HashTable *ht_insert(HashTable *hashtable, void *k, Type dt)
{
  if(hashtable == NULL)
  {
    printf("table is not defined!, create table and try again\n");
    return NULL;
  }
  unsigned int i = ht_hash(k, hashtable->table_count, dt);
  hashtable->table[i] = ll_insert(hashtable->table[i], k, dt);
  hashtable->cardinal++;
  hashtable->sorts = ll_insert(hashtable->sorts, &i, sizeof(i));
  return hashtable;
}

Node *ht_search(HashTable *hashtable, void *k, Type dt)
{
  unsigned int _i = ht_hash(k, hashtable->table_count, dt);
  Node *list = hashtable->table[_i];
  Node *result = ll_search( list, k, dt);
  return result;
}

bool ht_contain(HashTable *hashtable, void *k, Type dt)
{
  int i = ht_hash(k, hashtable->table_count, dt);
  Node *list = hashtable->table[i];
  Node *result = ll_search( list, k, dt);
  if(result == NULL)
    return false;
  return true;
}

void ht_free_table(HashTable *hashtable)
{
  if(hashtable)
  {
    if(hashtable->table)
    {
      free(hashtable->table);
    }
    free(hashtable);
  }
  return;
}

HashTable *ht_delete_node(HashTable *hashtable, void *k, Type dt)
{
  Node *item = ht_search(hashtable, k, dt);
  int i = ht_hash(k, hashtable->table_count, dt);
  if(item == NULL) return hashtable;

  item = ht_free_node(hashtable->table[i], item);
  hashtable->cardinal--;
  
  return hashtable;
}

static Node *ht_free_node(Node *list, Node* node)
{
  return ll_delete_by_node(list, node);
}

void ht_delete(HashTable *hashtable)
{
  Node *tmp=NULL;
  for(int i=0; i<hashtable->table_count; i++)
    hashtable->table[i] = ll_delete_entire_list(hashtable->table[i]);
  
  free(hashtable->table);
  hashtable->table=NULL;
  hashtable->table_count=0;
  hashtable->cardinal=0;
  free(hashtable);
  hashtable=NULL;
  
  return;
}

void ht_print(HashTable *hashtable, Type dt)
{
  if(hashtable==NULL)
  {
    printf("Table is empty!\n");
    return;
  }
  int i=0;
  unsigned int n=hashtable->table_count;
  while(i<n)
  {
    if(hashtable->table[i]==NULL) 
    {
      printf("[%2d]---->x\n", i);
      i++;
      continue;
    }
    printf("[%2d]---->", i);
    ll_print_list(hashtable->table[i], dt);
    printf("\n");
    i++;
  }
}


// some pointer functions that is needed in some operations

void ht_print_int(void *n)
{
  printf("|%d|", *(int*)n);
  return;
}

void ht_print_float(void *n)
{
  printf("|%.3Lf|", *(long double*)n);
}


unsigned int ht_hash_set(void *k, int m)
{
  Set s=*(Set*)k;
  int i = s.cardinal;
  if(i<0)
    i=i*-1;
  return i%m;
}

unsigned int ht_hash_int_point(void *k, int m)
{
  Point p=*(Point*)k;
  int x = *(int*)p.x;
  int y = *(int*)p.y;
  int r = x+y;

  if(r<0)
    r=r*-1;
  return r%m;
}
