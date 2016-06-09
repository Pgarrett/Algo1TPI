#include<iostream>
#include<sstream>

#include "aux.h"

using namespace std;

template <class T>
inline void mostrarSecuencia(std::ostream & os, const Secuencia<T> & s)
{
  os << "[";
  for(unsigned int i=0; i<s.size(); i++)
  {
    os << s[i];

    if(i < s.size()-1)
      os << ",";
  }
  os << "]";
}

template<class T>
unsigned int cuenta(const T &x, const std::vector<T> &v) {
    unsigned int cant = 0;
    for (unsigned int i = 0; i < v.size(); ++i) {
        if (x == v[i]) ++cant;
    }
    return cant;
}

template<class T>
bool mismos(const std::vector<T> &a, const std::vector<T> &b) {
    bool res = a.size() == b.size();
    for (unsigned int i = 0; res && i < a.size(); ++i) {
        res = cuenta(a[i], a) == cuenta(a[i], b);
    }
    return res;
}

template <class T>
inline void ordenar(Secuencia<T> & vs)
{
  for(unsigned int i=0; i<vs.size(); i++)
  {
    for(unsigned int j=0; j<vs.size()-1; j++)
    {
      if(vs[j] < vs[j+1])
        swap(vs[j], vs[j+1]);
    }
  }
}

inline string trim(string& str)
{
  size_t first = str.find_first_not_of(' ');
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last-first+1));
}

inline vector<string> splitVector(string str, char delimiter)
{
  vector<string> result;
  stringstream ss(str);
  string tok;
  while(getline(ss, tok, delimiter))
  {
    result.push_back(trim(tok));
  }
  return result;
}


