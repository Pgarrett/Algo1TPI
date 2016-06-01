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
  Dimension dimensionCampo = _campo.dimensiones();
  for(unsigned int i = 0; i < dimensionCampo.ancho; i++)
  {
    for(unsigned int j = 0; j < dimensionCampo.largo; j++)
    {
      vector<Posicion> vecinasConPlaga = parcelasVecinasConPlaga(Posicion(i, j));
      if(vecinasConPlaga.size() > 0)
        _estado.parcelas[i][j] = ConPlaga;
    }
  }
}

void Sistema::despegar(const Drone & d)
{
  Drone d1 = dronePorId(d.id());
  // Por requiere sabemos que hay alguna parcela libre.
  d1.moverA(parcelasVecinasLibres(posicionGranero())[0]);
}

bool Sistema::listoParaCosechar() const
{
  Dimension dimensionCampo = _campo.dimensiones();
  Secuencia<Posicion> parcelas = todasLasParcelas();

  int parcelasConCultivo = parcelas.size()-2; // // Menos la casa y el granero.
  int cultivosCosechables = 0;

  int i = 0;
  while(i < parcelas.size())
  {
    if(_estado.parcelas[parcelas[i].x][parcelas[i].y] == ListoParaCosechar)
      cultivosCosechables++;
    i++;
  }

  return (cultivosCosechables/parcelasConCultivo) >= 0,9;
}

void Sistema::aterrizarYCargarBaterias(Carga b)
{
  for(unsigned int i = 0; i < _enjambre.size(); i++)
  {
    if(_enjambre[i].bateria() < b)
    {
      Drone d = _enjambre[i];
      d.setBateria(100);
      d.moverA(posicionGranero());
      d.borrarVueloRealizado();
    }
  }
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

Secuencia<Posicion> Sistema::todasLasParcelas() const
{
  Dimension dimensionCampo = _campo.dimensiones();
  Secuencia<Posicion> parcelas;

  for(int i = 0; i < dimensionCampo.ancho; i++)
    for(int j = 0; j < dimensionCampo.largo; j++)
      parcelas.push_back(Posicion(i,j));

  return parcelas;
}

Posicion Sistema::posicionGranero()
{
  Dimension dimensionCampo = _campo.dimensiones();

  for(int i = 0; i < dimensionCampo.ancho; i++)
  {
    for(int j = 0; j < dimensionCampo.largo; j++)
    {
      Posicion p(i,j);
      if(_campo.contenido(p) == Granero)
        return p;
    }
  }
}

Secuencia<Posicion> Sistema::parcelasVecinas(Posicion p)
{
  Secuencia<Posicion> result;
  Dimension dimensionCampo = _campo.dimensiones();

  if(p.x-1 >= 0)
    result.push_back(Posicion(p.x-1, p.y));

  if(p.x+1 < dimensionCampo.ancho)
    result.push_back(Posicion(p.x+1, p.y));

  if(p.y-1 >= 0)
    result.push_back(Posicion(p.x, p.y-1));

  if(p.y+1 < dimensionCampo.largo)
    result.push_back(Posicion(p.x, p.y+1));
}

Secuencia<Posicion> Sistema::parcelasVecinasConPlaga(Posicion p)
{
  Secuencia<Posicion> result;
  Secuencia<Posicion> vecinas = parcelasVecinas(p);

  for(unsigned int i = 0; i < vecinas.size(); i++)
  {
    if(_estado.parcelas[vecinas[i].x][vecinas[i].y] == ConPlaga)
      result.push_back(vecinas[i]);
  }

  return result;
}

Secuencia<Posicion> Sistema::parcelasVecinasLibres(Posicion p)
{
  Secuencia<Posicion> parcelasLibres;
  Secuencia<Posicion> vecinas = parcelasVecinas(p);

  for(int i = 0; i< vecinas.size(); i++)
  {
    bool ocupada = false;
    for(int j = 0; j < _enjambre.size(); j++)
      if(_enjambre[j].enVuelo() && _enjambre[j].posicionActual() == vecinas[i])
          ocupada = true;

    if(!ocupada)
      parcelasLibres.push_back(vecinas[i]);
  }

  return parcelasLibres;
}

Drone Sistema::dronePorId(ID id)
{
  for(unsigned int i=0; i<_enjambre.size(); i++)
    if(_enjambre[i].id() == id)
      return _enjambre[i];
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
