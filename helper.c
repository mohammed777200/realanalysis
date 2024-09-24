#include "helper.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


int orderofnumber(int n)
{
  int o=0;
  int k=n;
  while(k!=0)
  {
    o++;
    k=k/10;
  }
  return o;
}

char inttochar(int n)
{
  char c='0'+n;
  return c;
}

int chartoint(char c)
{
  int n=c-'0';
  return n;
}

char* inttostr(int n)
{
  char *str=malloc(sizeof(char)*orderofnumber(n));
  int k=n;
  int i=orderofnumber(n)-1;
  int rem=0;
  while(i>=0)
  {
    rem=k%10;
    str[i] = inttochar(rem);
    k=k/10;
    i--;
  }
  
  return str;
}

int strtoint(char *str)
{
  int sum=0;
  int i=0;
  while(i<strlen(str))
  {
    sum+=chartoint(str[i])*pow(10, strlen(str)-i-1);
    i++;
  }

  return sum;
}




/* float strtofloat(char *str) */
/* { */
/*   float _i=0; */
/*   float _d=0; */
/* } */
