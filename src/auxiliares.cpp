#include<iostream>

#include "auxiliares.h"

using namespace std;

bool mismos(const Secuencia<Producto> & v1, const Secuencia<Producto> & v2)
{
  if(v1.size() != v2.size())
    return false;

  for(int i=0; i<v1.size(); i++)
  {
    if(cuenta(v1, v1[i]) != cuenta(v2, v1[i]))
      return false;
  }

  return true;
}

int cuenta(const Secuencia<Producto> & v, Producto elemento)
{
  int contador = 0;
  for(int i=0; i<v.size(); i++)
    if(v[i] == elemento)
      contador++;
  return contador;
}
