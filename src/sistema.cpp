#include <stdlib.h>
#include <sstream>

#include "sistema.h"
#include "auxiliares.cpp"

using namespace std;

Sistema::Sistema()
{
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
  _campo = c;
  _enjambre = ds;
  _estado = Grilla<EstadoCultivo>(c.dimensiones());

  // Sabemos que por invariante el campo tiene al menos una parcela, por lo tanto al menos alguna fila y columna
  // por eso podemos pedir parcelas[0]
  for(unsigned int i=0; i<_estado.parcelas.size(); i++)
    for(unsigned int j=0; j<_estado.parcelas[0].size(); j++)
      _estado.parcelas[i][j] = NoSensado;
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
  for(unsigned int i = 0; i < _enjambre.size(); i++)
  {
    _enjambre[i].mostrar(os);
  }
  os << " }";
}

//{ S { C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]}
// [{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]},
// { D 15 46 [[0,1],[1,1][2,1][2,2]] [HerbicidaLargoAlcance, Fertilizante, Herbicida, Plaguicida]}]
// [[NoSensado,EnCrecimiento,NoSensado], [ConMaleza,NoSensado,ConPlaga], [EnCrecimiento,ListoParaCosechar, ConPlaga]] }
void Sistema::guardar(std::ostream & os) const
{
  os << "{ S ";
  _campo.guardar(os);

  os << " [";
  for(unsigned int i = 0; i < _enjambre.size(); i++)
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
    Posicion p1 = Posicion(i,0);
    os << estadoDelCultivo(p1);
    for(int j=1; j<d.largo; j++)
    {
      Posicion p2 = Posicion(i, 0);
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
  _campo = Campo();
  _enjambre.clear();

  char b;
  is >> b >> b;
  _campo.cargar(is);

  string todosLosDrones;
  getline(is, todosLosDrones, ']');
  vector<string> drones = splitVector(todosLosDrones, ',');

  Dimension dimensionCampo = _campo.dimensiones();
  _estado = Grilla<EstadoCultivo>(dimensionCampo);
  for(int i=0; i<dimensionCampo.ancho; i++)
  {
    is >> b;
    string estadoC;
    for(int j=0; j<dimensionCampo.largo; j++)
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

      Posicion p(i,j);
      // Dado que no esta especificado que estado tienen las parcelas que no son de cultivo, le ponemos NoSensado
      if(_campo.contenido(p) != Cultivo)
        _estado.parcelas[i][j] = NoSensado;
      else
        _estado.parcelas[i][j] = estadoDelCultivo(p);

      string parcela;
      getline(is, parcela, ',');
    }
    is >> b;
  }
}

bool Sistema::operator==(const Sistema & otroSistema) const
{
  if(!(_campo == otroSistema.campo())
     || !mismos(_enjambre, otroSistema.enjambreDrones()))
      return false;

  // Ya sabemos que |estadoDelCultivo| es igual para ambos sistemas, por igualdad de campo + invariante
  for(unsigned int i=0; i<_estado.parcelas.size(); i++)
    for(unsigned int j=0; j<_estado.parcelas[0].size(); j++)
      if(_estado.parcelas[i][j] != otroSistema.estadoDelCultivo(Posicion(i,j)))
        return false;

  return true;
}

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
  s.mostrar(os);
	return os;
}
