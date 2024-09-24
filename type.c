// type.c

#include "type.h"

void *alloc_type(Type dt, unsigned int n)
{
  void *obj=(void*)calloc(n, dt_size(dt));
  if(!obj) { printf("failed to allocate memory for %d data type!\n", dt); free(obj); obj=NULL; }
  return obj;
}


unsigned int dt_size(Type dt)
{
  unsigned int data_size=0;
  switch (dt)
  {
// primitive data types
    case CHAR:
      data_size=sizeof(char);
      break;
    case INT:
      data_size=sizeof(int);
      break;
    case FLOAT:
      data_size=sizeof(float);
      break;
    case DOUBLE:
      data_size=sizeof(double);
      break;
    case LONG_DOUBLE:
      data_size=sizeof(long double);
      break;
    case U_INT:
      data_size=sizeof(unsigned int);
      break;
    case BOOL:
      data_size=sizeof(bool);
      break;
// structure defined data types
    case SET:
      data_size=sizeof(Set);
      break;
    case GRAPH:
      data_size=sizeof(Graph);
      break;
    case HASHTABLE:
      data_size=sizeof(HashTable);
      break;
    case NODE:
      data_size=sizeof(Node);
      break;
    case POINT:
      data_size=sizeof(Point);
      break;
    case FUNCTION:
      data_size=sizeof(Function);
      break;
    case TERM:
      data_size=sizeof(Term);
      break;
    case POLYNOMIAL:
      data_size=sizeof(Polynomial);
      break;
    default:
      data_size=sizeof(char);
  }
  return data_size;
}
