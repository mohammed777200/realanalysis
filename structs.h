// structs.h

#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdbool.h>

typedef struct Node
{
  void *data;
  unsigned int size;
  struct Node *next;
}Node;

typedef struct HashTable
{
  Node **table;
  unsigned int table_count; // size of table, (number of buckets)
  unsigned int cardinal; // count of how many keys listed in hashtable
  Node *sorts;
}HashTable;

typedef enum Type
{
// primitive data types
  CHAR,
  INT,
  FLOAT,
  DOUBLE,
  LONG_DOUBLE,
  U_INT,
  BOOL,
// structure data types
  SET,
  GRAPH,
  HASHTABLE,
  NODE,
  POINT,
  FUNCTION,
  POLYNOMIAL,
  TERM,
}Type;

typedef struct Set
{
  HashTable *members; // malloc then free, hence malloc(sizeof(HashTable)) and then free(members)
  unsigned int cardinal;
  Type data_type;
  void *delta;
}Set;

typedef struct Graph 
{
  Set *domain;
  Set *codomain;
}Graph;

typedef struct Point
{
  void *x;
  void *y;
}Point;

typedef struct Term
{
  double power;
  double coefficient;
  double x_pos;
}Term;

typedef struct Polynomial
{
  // abstract linkedlist for holding type Term structure
  Node *terms;
  // polynomial string
  char *polynomial;
}Polynomial;

typedef struct Function
{
  Set *domain;
  Set *codomain;
  Polynomial *polynomial;
  void *solution;
}Function;

#endif
