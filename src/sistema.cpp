#include<sstream>
#include "sistema.h"
#include "auxiliares.cpp"
#include <stdlib.h>
#include <sstream>

using namespace std;

Sistema::Sistema()
{
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
  _campo = c;
  Dimension dimensionCampo = _campo.dimensiones();
  _estado = Grilla<EstadoCultivo>(dimensionCampo);
  for(int i=0; i<dimensionCampo.ancho; i++)
  {
    _estado.parcelas[i][0] = NoSensado;
    for(int j=1; j<dimensionCampo.largo; j++)
    {
      _estado.parcelas[i][j] = NoSensado;
    }
  }
  _enjambre = ds;
}

const Campo & Sistema::campo() const
{
  return _campo;
}

EstadoCultivo Sistema::estadoDelCultivo(const Posicion & p) const
{
  return _estado.parcelas[p.x][p.y];
}

const Secuencia<Drone>& Sistema::enjambreDrones() const
{
  return _enjambre;
}

void Sistema::crecer()
{
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
}

void Sistema::seExpandePlaga()
{
}

void Sistema::despegar(const Drone & d)
{
}

bool Sistema::listoParaCosechar() const
{
	return false;
}

void Sistema::aterrizarYCargarBaterias(Carga b)
{
}

void Sistema::fertilizarPorFilas()
{
}

void Sistema::volarYSensar(const Drone & d)
{
}

void Sistema::mostrar(std::ostream & os) const
{
  os << "{ S ";
  _campo.mostrar(os);
  for(int i = 0; i < _enjambre.size(); i++)
  {
    _enjambre[i].mostrar(os);
  }
  os << " }";
}

//{ S { C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]} [{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]}, { D 15 46 [[0,1],[1,1][2,1][2,2]] [HerbicidaLargoAlcance, Fertilizante, Herbicida, Plaguicida]}] [[NoSensado,EnCrecimiento,NoSensado], [ConMaleza,NoSensado,ConPlaga], [EnCrecimiento,ListoParaCosechar, ConPlaga]] }
void Sistema::guardar(std::ostream & os) const
{
  os << "{ S ";
  _campo.guardar(os);
  os << " [";
  for(int i = 0; i < _enjambre.size(); i++)
  {
    _enjambre[i].guardar(os);
    if (i < _enjambre.size() - 1)
      os << ",";
  }
  os << "]";
  Dimension d = _campo.dimensiones();
  for(int i=0; i<d.ancho; i++)
  {
    os << "[";
    Posicion p1 = Posicion();
    p1.x = i;
    p1.y = 0;
    os << estadoDelCultivo(p1);
    for(int j=1; j<d.largo; j++)
    {
      Posicion p2 = Posicion();
      p2.x = i;
      p2.y = 0;
      os << "," << estadoDelCultivo(p2);
    }
    if(i<d.ancho-1)
      os << "], ";
  }
  os << "]]}";
}

// { S
//  { C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]}
//  [{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]},
//   { D 15 46 [[0,1],[1,1][2,1][2,2]] [HerbicidaLargoAlcance, Fertilizante, Herbicida, Plaguicida]}
//  ]
//  [[NoSensado,EnCrecimiento,NoSensado], [ConMaleza,NoSensado,ConPlaga], [EnCrecimiento,ListoParaCosechar, ConPlaga]] }
void Sistema::cargar(std::istream & is)
{
  char b;
  is >> b >> b;
  this->_campo.cargar(is);
  is >> b;
  string ingresaDrones;
  getline(is, ingresaDrones, '[');
  char ultimoDrone = *ingresaDrones.rbegin();
  while(ultimoDrone != ']' && b != ']')
  {
    string d;
    getline(is, d, '}');
    d = d + '}';
    stringstream dStream(d);
    Drone droneCargado;
    droneCargado.cargar(dStream);
    this->_enjambre.push_back(droneCargado);
    is >> b;
    ultimoDrone = *d.rbegin();
  }
  is >> b;
  Dimension dimensionCampo = _campo.dimensiones();
  this->_estado = Grilla<EstadoCultivo>(dimensionCampo);
  for(int i=0; i<dimensionCampo.ancho; i++)
  {
    is >> b;
    string estadoC;
    Posicion p1;
    p1.x = i;
    p1.y = 0;
    getline(is, estadoC, ',');
    _estado.parcelas[i][0] = getEstadoCultivo(p1, _campo.contenido(p1), estadoC);
    for(int j=1; j<dimensionCampo.largo; j++)
    {
      if (j < dimensionCampo.largo - 1)
      {
        getline(is, estadoC, ',');
      }
      else
      {
        getline(is, estadoC, ']');
        is >> b;
      }
      Posicion p2;
      p2.x = i;
      p2.y = j;
      _estado.parcelas[i][j] = getEstadoCultivo(p2, _campo.contenido(p2), estadoC);
    }
  }
  is >> b;
}

bool Sistema::operator==(const Sistema & otroSistema) const
{
	return false;
}

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	return os;
}
