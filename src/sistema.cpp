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
  Secuencia<Posicion> parcelas = todasLasParcelasConCultivo();
  Grilla<EstadoCultivo> estadoPrevio = _estado;
  for(int i = 0; i < parcelas.size(); i++)
  {
    vector<Posicion> vecinasConPlaga = parcelasVecinasConPlaga(parcelas[i], estadoPrevio);
    if(vecinasConPlaga.size() > 0)
      _estado.parcelas[parcelas[i].x][parcelas[i].y] = ConPlaga;
  }
}

void Sistema::despegar(const Drone & d)
{
  Drone & d1 = dronePorId(d.id());
  // Por requiere sabemos que hay alguna parcela libre.
  d1.moverA(parcelasVecinasLibres(posicionGranero())[0]);
}

bool Sistema::listoParaCosechar() const
{
  Dimension dimensionCampo = _campo.dimensiones();
  Secuencia<Posicion> parcelas = todasLasParcelasConCultivo();

  int parcelasConCultivo = parcelas.size();
  int cultivosCosechables = 0;

  unsigned int i = 0;
  while(i < parcelas.size())
  {
    if(estadoDelCultivo(parcelas[i]) == ListoParaCosechar)
      cultivosCosechables++;
    i++;
  }
  
  return ((double)cultivosCosechables/(double)parcelasConCultivo) >= 0.9;
}

void Sistema::aterrizarYCargarBaterias(Carga b)
{
  for(unsigned int i = 0; i < _enjambre.size(); i++)
  {
    if(_enjambre[i].bateria() < b)
    {
      Drone& d = _enjambre[i];
      d.setBateria(100);
      d.moverA(posicionGranero());
      d.borrarVueloRealizado();
    }
  }
}

void Sistema::fertilizarPorFilas()
{
  for(unsigned int i=0; i<_enjambre.size(); i++)
  {
    Drone& d = _enjambre[i];
    if(d.enVuelo())
      fertilizarFila(d);
  }
}

void Sistema::volarYSensar(const Drone & d)
{
  Drone & d1 = dronePorId(d.id());

  // Sabemos que hay al menos una parcela libre por requiere.
  Posicion posicion = parcelasVecinasLibres(d1.posicionActual())[0];
  d1.moverA(posicion);
  d1.setBateria(d1.bateria()-1);
  
  if(_campo.contenido(posicion) != Cultivo) return;
  if(estadoDelCultivo(posicion) == NoSensado)
  {
    // Ponemos cualquier estado distinto de NoSensado
    _estado.parcelas[posicion.x][posicion.y] = RecienSembrado;
  }
  
  Secuencia<Producto> productos = productosAplicables(estadoDelCultivo(posicion));
  if(productos.size() > 0)
    aplicarProductoEnPosicionActual(d1, productos[0]);
}

void Sistema::mostrar(std::ostream & os) const
{
  os << "{ S ";
  _campo.guardar(os);

  os << " [";
  for(unsigned int i = 0; i < _enjambre.size(); i++)
  {
    _enjambre[i].guardar(os);
    if (i < _enjambre.size()-1)
      os << ", ";
  }
  os << "]";
  os << "]}";
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
      os << ", ";
  }
  os << "] [";

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

// { S{ C [3,3] [[Cultivo,Cultivo,Granero],[Cultivo,Casa,Cultivo],[Cultivo,Cultivo,Cultivo]]}
//[{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [PlaguicidaBajoConsumo,Herbicida,Fertilizante] true [2,0]},
//{ D 15 46 [[0,1],[1,1],[2,1],[2,2]] [HerbicidaLargoAlcance,Fertilizante,Plaguicida] true [2,2]}]
//[[NoSensado,EnCrecimiento,NoSensado],[ConMaleza,NoSensado,ConPlaga],[EnCrecimiento,ListoParaCosechar,ConPlaga]]}
void Sistema::cargar(std::istream & is)
{
  _campo = Campo();
  _enjambre.clear();

  char b;
  is >> b >> b;
  _campo.cargar(is);

  is >> b >> b >> b;
  while(b != ']')
  {
    string drone;
    getline(is, drone, '}');
    stringstream ss(drone);
    Drone d;
    d.cargar(ss);
    _enjambre.push_back(d);
    is >> b;
  }

  Dimension dimensionCampo = _campo.dimensiones();
  _estado = Grilla<EstadoCultivo>(dimensionCampo);
  is >> b >> b;
  for(int i=0; i<dimensionCampo.ancho; i++)
  {
    //is >> b;
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

      // Dado que no esta especificado que estado tienen las parcelas que no son de cultivo, le ponemos NoSensado
      if(_campo.contenido(Posicion(i,j)) != Cultivo)
        _estado.parcelas[i][j] = NoSensado;
      else
        _estado.parcelas[i][j] = estadoCultivo(estadoC);

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

Secuencia<Posicion> Sistema::todasLasParcelasConCultivo() const
{
  Dimension dimensionCampo = _campo.dimensiones();
  Secuencia<Posicion> parcelas;

  for(int i = 0; i < dimensionCampo.ancho; i++)
  {
    for(int j = 0; j < dimensionCampo.largo; j++)
    {
      Posicion p = Posicion(i,j);
      if(_campo.contenido(p) == Cultivo)
        parcelas.push_back(p);
    }
  }

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
  Secuencia<Posicion> result = Secuencia<Posicion>();
  Dimension dimensionCampo = _campo.dimensiones();

  if(p.x-1 >= 0)
    result.push_back(Posicion(p.x-1, p.y));

  if(p.x+1 < dimensionCampo.ancho)
    result.push_back(Posicion(p.x+1, p.y));

  if(p.y-1 >= 0)
    result.push_back(Posicion(p.x, p.y-1));

  if(p.y+1 < dimensionCampo.largo)
    result.push_back(Posicion(p.x, p.y+1));

  return result;
}

Secuencia<Posicion> Sistema::parcelasVecinasConCultivo(Posicion p)
{
  Secuencia<Posicion> vecinas = parcelasVecinas(p);
  Secuencia<Posicion> result = Secuencia<Posicion>();

  for(unsigned int i=0; i<vecinas.size(); i++)
    if(_campo.contenido(vecinas[i]) == Cultivo)
      result.push_back(vecinas[i]);

  return result;
}

Secuencia<Posicion> Sistema::parcelasVecinasConPlaga(Posicion p, Grilla<EstadoCultivo> estadoPrevio)
{
  Secuencia<Posicion> result = Secuencia<Posicion>();
  Secuencia<Posicion> vecinas = parcelasVecinasConCultivo(p);

  for(unsigned int i = 0; i < vecinas.size(); i++)
  {
    if(estadoPrevio.parcelas[vecinas[i].x][vecinas[i].y] == ConPlaga)
      result.push_back(vecinas[i]);
  }

  return result;
}

Secuencia<Posicion> Sistema::parcelasVecinasConMaleza(Posicion p)
{
  Secuencia<Posicion> result;
  Secuencia<Posicion> vecinas = parcelasVecinasConCultivo(p);

  for(unsigned int i = 0; i < vecinas.size(); i++)
  {
    if(_estado.parcelas[vecinas[i].x][vecinas[i].y] == ConMaleza)
      result.push_back(vecinas[i]);
  }

  return result;
}

Secuencia<Posicion> Sistema::parcelasVecinasLibres(Posicion p)
{
  Secuencia<Posicion> parcelasLibres;
  Secuencia<Posicion> vecinas = parcelasVecinas(p);

  for(unsigned int i = 0; i< vecinas.size(); i++)
  {
    bool ocupada = false;
    for(unsigned int j = 0; j < _enjambre.size(); j++)
    {
      if(_enjambre[j].enVuelo() && _enjambre[j].posicionActual() == vecinas[i])
          ocupada = true;
    }

    if(!ocupada)
      parcelasLibres.push_back(vecinas[i]);
  }

  return parcelasLibres;
}

Drone & Sistema::dronePorId(ID id)
{
  for(unsigned int i=0; i<_enjambre.size(); i++)
    if(_enjambre[i].id() == id)
      return _enjambre[i];
}

void Sistema::fertilizarFila(Drone& d)
{
  if(tieneProducto(d, Fertilizante) && _campo.contenido(d.posicionActual()) == Cultivo)
    fertilizarPosicionActual(d);

  while(d.posicionActual().x > 0 && tieneProducto(d, Fertilizante) && d.bateria() > 0
        && _campo.contenido(d.posicionActual()) == Cultivo)
  {
    d.moverA(d.posicionActual()-Posicion(1,0));
    d.setBateria(d.bateria()-1);

    if(_campo.contenido(d.posicionActual()) == Cultivo)
      fertilizarPosicionActual(d);
  }
}

void Sistema::aplicarProductoEnPosicionActual(Drone d, Producto p)
{
  d.sacarProducto(p);
  Posicion pos = d.posicionActual();
  d.setBateria(d.bateria() - consumoDeBateria(p));
  _estado.parcelas[pos.x][pos.y] = estadoFinal(p);

  if(p == HerbicidaLargoAlcance)
  {
    Secuencia<Posicion> adyacentes = parcelasVecinasConMaleza(pos);
    for(unsigned int i=0; i<adyacentes.size(); i++)
      _estado.parcelas[adyacentes[i].x][adyacentes[i].y] = EnCrecimiento;
  }
}

void Sistema::fertilizarPosicionActual(Drone d)
{
  d.sacarProducto(Fertilizante);
  Posicion pos = d.posicionActual();

  if(estadoDelCultivo(pos) == RecienSembrado || estadoDelCultivo(pos) == EnCrecimiento)
    _estado.parcelas[pos.x][pos.y] = ListoParaCosechar;
}

// Lo definimos aca porque la consigna dice que no podemos modificar la parte publica de las clases.
// En realidad iria en Drone.
bool Sistema::tieneProducto(Drone d, Producto p)
{
  Secuencia<Producto> productos = d.productosDisponibles();
  for(unsigned int i=0; i<productos.size(); i++)
    if(productos[i] == p)
      return true;
  return false;
}

Secuencia<Producto> Sistema::productosAplicables(EstadoCultivo ec)
{
  Secuencia<Producto> fertilizantes;
  fertilizantes.push_back(Fertilizante);
  if(ec == RecienSembrado || ec == EnCrecimiento) return fertilizantes;

  Secuencia<Producto> plaguicidas;
  plaguicidas.push_back(Plaguicida);
  plaguicidas.push_back(PlaguicidaBajoConsumo);
  if(ec == ConPlaga) return plaguicidas;

  Secuencia<Producto> herbicidas;
  plaguicidas.push_back(Herbicida);
  plaguicidas.push_back(HerbicidaLargoAlcance);
  if(ec == ConMaleza) return herbicidas;
  
  return Secuencia<Producto>();
}

int Sistema::consumoDeBateria(Producto p)
{
  if(p == Fertilizante) return 0;
  else if(p == Plaguicida) return 10;
  else return 5;
}

EstadoCultivo Sistema::estadoFinal(Producto p)
{
  if(p == Fertilizante) return ListoParaCosechar;
  else return RecienSembrado;
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
