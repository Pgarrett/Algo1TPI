#include<iostream>
#include<fstream>
#include "campo.h"

using namespace std;

Campo::Campo()
{
}

Campo::Campo(const Posicion &posG, const Posicion &posC, Dimension dimension) {
    _dimension = dimension;
    _grilla = Grilla<Parcela>(_dimension);
    _grilla.parcelas[posG.x][posG.y] = Granero;
    _grilla.parcelas[posC.x][posC.y] = Casa;
}

Campo::Campo(const Posicion &posG, const Posicion &posC)
{
  _dimension.ancho = max(posG.x, posC.x)+1;
  _dimension.largo = max(posG.y, posC.y)+1;
  // Si el campo tiene solo dos parcelas, agregamos una fila para no romper el invariante AlgoDeCultivo.
  if(_dimension.ancho * _dimension.largo == 2)
    _dimension.largo++;

  _grilla = Grilla<Parcela>(_dimension);

  for(int i=0; i<_dimension.ancho; i++)
  {
    for(int j=0; j<_dimension.largo; j++)
    {
      _grilla.parcelas[posG.x][posG.y] = Cultivo;
    }
  }
  _grilla.parcelas[posG.x][posG.y] = Granero;
  _grilla.parcelas[posC.x][posC.y] = Casa;
}

Dimension Campo::dimensiones() const
{
  return _dimension;
}

Parcela Campo::contenido(const Posicion &p) const
{
	return _grilla.parcelas[p.x][p.y];
}

void Campo::mostrar(std::ostream &os) const
{
  os << "{ C ";
  os << "[" << _dimension.ancho << "," << _dimension.largo << "]}";
}

// { C [3,3] [[Cultivo,Cultivo,Granero],[Cultivo,Casa,Cultivo],[Cultivo,Cultivo,Cultivo]]}
void Campo::guardar(std::ostream &os) const
{
  os << "{ C [" << _dimension.ancho << "," << _dimension.largo << "] ";

  os << "[";
  for(int i=0; i<_dimension.ancho; i++)
  {
    os << "[" << _grilla.parcelas[i][0];

    for(int j=1; j<_dimension.largo; j++)
    {
      os << "," << _grilla.parcelas[i][j];
    }

    if(i<_dimension.ancho-1)
      os << "],";
  }

  os << "]]}";
}

void Campo::cargar(std::istream &is)
{
  char b; // Para guardar basura / caracteres que no nos interesan.
  is >> b >> b >> b >> _dimension.ancho >> b >> _dimension.largo >> b >> b;
  _grilla = Grilla<Parcela>(_dimension);
  for(int i=0; i<_dimension.ancho; i++)
  {
    is >> b;
    for(int j=0; j<_dimension.largo; j++)
    {
      string parcela;
      if (j < _dimension.largo - 1)
        getline(is, parcela, ',');
      else
        getline(is, parcela, ']');

      _grilla.parcelas[i][j] = tipoDeParcela(parcela);
    }
    is >> b;
  }
}

bool Campo::operator==(const Campo &otroCampo) const
{
	if(_dimension.ancho != otroCampo.dimensiones().ancho
    || _dimension.largo != otroCampo.dimensiones().largo)
    return false;

  for(int i=0; i<_dimension.ancho; i++)
  {
    for(int j=0; j<_dimension.largo; j++)
    {
      Posicion p(i,j);
      if(contenido(p) != otroCampo.contenido(p))
        return false;
    }
  }
  return true;
}

std::ostream & operator<<(std::ostream &os, const Campo &c)
{
  c.mostrar(os);
	return os;
}
