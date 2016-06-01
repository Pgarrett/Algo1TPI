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
  for(unsigned int i=0; i<_estado.parcelas.size(); i++)
  {
    for(unsigned int j=0; j<_estado.parcelas[0].size(); j++)
    {
      if(_estado.parcelas[i][j] == RecienSembrado)
        _estado.parcelas[i][j] = EnCrecimiento;
      else if(_estado.parcelas[i][j] == EnCrecimiento)
        _estado.parcelas[i][j] = ListoParaCosechar;
    }
  }
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
  for(unsigned int i=0; i<ps.size(); i++)
    _estado.parcelas[ps[i].x][ps[i].y] = ConMaleza;
}

void Sistema::seExpandePlaga()
{
}

void Sistema::despegar(const Drone & d)
{
  Dimension dimensionCampo = _campo.dimensiones();
  Posicion posicionGranero = damePosicionGranero(dimensionCampo);
  vector<Posicion> cultivosAlLadoDelGranero = dameCultivosVecinosAlGranero(dimensionCampo, posicionGranero);
  vector<Posicion> parcelasVecinasConDrone = dameParcelasVecinasConDrone(cultivosAlLadoDelGranero, d.id());
  vector<Posicion> posicionesValidasParaDrone;
  for(int i = 0; i < cultivosAlLadoDelGranero.size(); i++)
  {
    if(noHayDroneAca(cultivosAlLadoDelGranero[i], parcelasVecinasConDrone))
      posicionesValidasParaDrone.push_back(cultivosAlLadoDelGranero[i]);
  }
  if(posicionesValidasParaDrone.size() > 0)
  {
    for(int i = 0; i < _enjambre.size(); i++)
    {
      if(_enjambre[i].id() == d.id())
        _enjambre[i].moverA(posicionesValidasParaDrone[0]);
    }
  }
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
    if (i < _enjambre.size()-1)
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
      Posicion p2 = Posicion(i,j);
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

Posicion Sistema::damePosicionGranero(Dimension dimensionCampo)
{
  for(int i = 0; i < dimensionCampo.ancho; i++)
  {
    for(int j = 0; j < dimensionCampo.largo; j++)
    {
      Posicion aux(i,j);
      if(this->_campo.contenido(aux) == Granero)
        return aux;
    }
  }
}

vector<Posicion> Sistema::dameCultivosVecinosAlGranero(Dimension dimensionCampo, Posicion posicionGranero)
{
  vector<Posicion> result;
  for(int i = 0; i < dimensionCampo.ancho; i++)
  {
    for(int j = 0; j < dimensionCampo.largo; j++)
    {
      if((i == posicionGranero.x - 1 || i == posicionGranero.x + 1) && j == posicionGranero.y)
      {
        Posicion aux(i, j);
        if(_campo.contenido(aux) == Cultivo)
          result.push_back(aux);
      }
    }
  }
  return result;
}

vector<Posicion> Sistema::dameParcelasVecinasConDrone(vector<Posicion> cultivosAlLadoDelGranero, ID droneId)
{
  vector<Posicion> result;
  for(int i = 0; i < cultivosAlLadoDelGranero.size(); i++)
  {
    for(int j = 0; j < _enjambre.size(); j++)
    {
      if(_enjambre[j].id() != droneId && _enjambre[j].enVuelo() && _enjambre[j].posicionActual() == cultivosAlLadoDelGranero[i])
          result.push_back(cultivosAlLadoDelGranero[i]);
    }
  }
  return result;
}

bool Sistema::noHayDroneAca(Posicion p, vector<Posicion> parcelasVecinasConDrone)
{
  bool result = true;
  for(int j = 0; j <parcelasVecinasConDrone.size(); j++)
  {
    result &= (p.x != parcelasVecinasConDrone[j].x && p.y != parcelasVecinasConDrone[j].y);
  }
  return result;
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
