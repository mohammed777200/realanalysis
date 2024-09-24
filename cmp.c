
#include "cmp.h"

#define _CMP(type, a, b)\
  type _a=*(type*)a;\
  type _b=*(type*)b;\
  _t=(_a==_b)?true:false;\


bool cmp_cmp(void *a, void *b, Type dt)
{
  bool _t=false;
  switch(dt)
  {
    case CHAR:
      {
        _CMP(char, a, b)
      }
      break;
    case INT:
      {
        _CMP(int, a, b)
      }
      break;
    case FLOAT:
      {
        _CMP(float, a, b)
      }
      break;
    case DOUBLE:
      {
        _CMP(double, a, b)
      }
      break;
    case U_INT:
      {
        _CMP(unsigned int, a, b)
      }
      break;
    case BOOL:
      {
        _CMP(bool, a, b)
      }
      break;
    default:
      {
        _CMP(int, a, b)
      }
  }
  return _t;
}
