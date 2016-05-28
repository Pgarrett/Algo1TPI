#include <iostream>
#include <fstream>
#include "campo.h"
#include <string.h>


using namespace std;

Campo::Campo()
{
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
  os << "[" << _dimension.ancho << "," << _dimension.largo << "] }";
}

void Campo::guardar(std::ostream &os) const
{
  os << "{ C ";
  os << "[" << _dimension.ancho << "," << _dimension.largo << "] ";
  os << "[";

  for(int i=0; i<_dimension.ancho; i++)
  {
    os << "[";
    os << _grilla.parcelas[i][0];
    for(int j=1; j<_dimension.largo; j++)
    {
      os << "," << _grilla.parcelas[i][j];
    }
    os << "]";
    if (i < _dimension.ancho - 1)
    {
      os << ", ";
    }
  }

  os << "]}";
}

void Campo::cargar(std::istream &is)
{
  string campo;
  getline(is, campo, 'C');
  getline(is, campo, '[');
  string dimensionAncho;
  getline(is, dimensionAncho, ',');
  this->_dimension.ancho = atoi(dimensionAncho.c_str());
  string dimensionLargo;
  getline(is, dimensionLargo, ']');
  this->_dimension.largo = atoi(dimensionLargo.c_str());
  string basura;
  getline(is, basura, '[');
  string finDeParcelas;
  char ultimaParcela = *basura.rbegin();
  this->_grilla = Grilla<Parcela>(this->_dimension);
  while (ultimaParcela != '}')
  {
    for (int i = 0; i < atoi(dimensionAncho.c_str()); i++)
    {
      getline(is, basura, '[');
      string parcela;
      getline(is, parcela, ',');
      _grilla.parcelas[i][0] = tipoDeParcela(parcela);
      for (int j = 1; j < atoi(dimensionLargo.c_str()); j++)
      {
        if (j < (atoi(dimensionLargo.c_str()) - 1))
        {
          getline(is, parcela, ',');
        }
        else
        {
          getline(is, parcela, ']');
        }
        _grilla.parcelas[i][j] = tipoDeParcela(parcela);
      }
    }
    getline(is, finDeParcelas, ' ');
    ultimaParcela = *finDeParcelas.rbegin();
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
      Posicion p;
      p.x = i;
      p.y = j;

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

std::ostream & operator<<(std::ostream &os, const Parcela &p)
{
  if(p == Casa) os << "Casa";
  if(p == Granero) os << "Granero";
  if(p == Cultivo) os << "Cultivo";
	return os;
}

std::ostream & operator<<(std::ostream &os, const Producto &p)
{
  if(p == Fertilizante) os << "Fertilizante";
  if(p == Plaguicida) os << "Plaguicida";
  if(p == PlaguicidaBajoConsumo) os << "PlaguicidaBajoConsumo";
  if(p == Herbicida) os << "Herbicida";
  if(p == HerbicidaLargoAlcance) os << "HerbicidaLargoAlcance";
	return os;
}

std::ostream & operator<<(std::ostream &os, const EstadoCultivo &e)
{
  if(e == RecienSembrado) os << "RecienSembrado";
  if(e == EnCrecimiento) os << "EnCrecimiento";
  if(e == ListoParaCosechar) os << "ListoParaCosechar";
  if(e == ConMaleza) os << "ConMaleza";
  if(e == ConPlaga) os << "ConPlaga";
  if(e == NoSensado) os << "NoSensado";
	return os;
}

Parcela tipoDeParcela(string p)
{
  if (p == "Cultivo") return Cultivo;
  if (p == "Granero") return Granero;
  if (p == "Casa") return Casa;
}
