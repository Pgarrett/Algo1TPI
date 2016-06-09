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
  for(int i = 0; i < dimensionCampo.ancho; i++)
  {
    for(int j = 0; j < dimensionCampo.largo; j++)
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

  return (cultivosCosechables/parcelasConCultivo) >= 0.9;
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
  for(int i=0; i<_enjambre.size(); i++)
  {
    Drone d = _enjambre[i];
    if(d.enVuelo())
    {
      fertilizarFila(d);
    }
  }
}

void Sistema::volarYSensar(const Drone & d)
{
  Drone d1 = dronePorId(d.id());
  // Por requiere sabemos que hay alguna parcela libre.
  Posicion nuevaPosicion = parcelasVecinasLibres(d.posicionActual())[0];
  d1.moverA(nuevaPosicion);
  if(_campo.contenido(nuevaPosicion) != Cultivo || estadoDelCultivo(nuevaPosicion) == NoSensado)
  {
    if(_campo.contenido(nuevaPosicion) == Cultivo)
      _estado.parcelas[nuevaPosicion.x][nuevaPosicion.y] = ListoParaCosechar;
    d1.setBateria(d.bateria() - 1);
  }
  else
  {
    vector<Producto> prodUsables = productosUsables(d1, estadoDelCultivo(nuevaPosicion));
    if(prodUsables.size() == 0)
      d1.setBateria(d.bateria() - 1);
    else
    {
      Producto productoAUsar = prodUsables[0];
      d1.setBateria(d.bateria() - cuantoConsumeP(productoAUsar));
      d1.sacarProducto(productoAUsar);
      for(unsigned int i = 0; i < todasLasParcelasConCultivo().size(); i++)
      {
        vector<Posicion> parcelasAdyacentes = parcelasVecinas(nuevaPosicion);
        parcelasAdyacentes.push_back(nuevaPosicion);
        for(unsigned int j = 0; j < parcelasAdyacentes.size() + 1; j++)
        {
          if(todasLasParcelasConCultivo()[i] == parcelasAdyacentes[j] && sePuedeAplicarP(productoAUsar, estadoDelCultivo(nuevaPosicion), d1))
          {
            if(productoAUsar == Fertilizante)
              _estado.parcelas[nuevaPosicion.x][nuevaPosicion.y] = ListoParaCosechar;
            else
              _estado.parcelas[nuevaPosicion.x][nuevaPosicion.y] = RecienSembrado;
          }
        }
      }
    }
  }
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

      Posicion p(i,j);
      // Dado que no esta especificado que estado tienen las parcelas que no son de cultivo, le ponemos NoSensado
      if(_campo.contenido(p) != Cultivo)
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

void Sistema::fertilizarFila(Drone d)
{
  if(tieneFertilizante(d) && _campo.contenido(d.posicionActual()) == Cultivo)
    fertilizarPosicionActual(d);

  while(d.posicionActual().x > 0 && tieneFertilizante(d) && d.bateria() > 0
        && _campo.contenido(d.posicionActual()) == Cultivo)
  {
    d.moverA(d.posicionActual()-Posicion(1,0));
    d.setBateria(d.bateria()-1);

    if(_campo.contenido(d.posicionActual()) == Cultivo)
      fertilizarPosicionActual(d);
  }
}

// Lo definimos aca porque la consigna dice que no podemos modificar la parte publica de las clases.
// En realidad iria en Drone.
bool Sistema::tieneFertilizante(Drone d)
{
  Secuencia<Producto> productos = d.productosDisponibles();
  for(int i=0; i<productos.size(); i++)
    if(productos[i] == Fertilizante)
      return true;
  return false;
}

void Sistema::fertilizarPosicionActual(Drone d)
{
  Secuencia<Producto> productos = d.productosDisponibles();
  for(int i=0; i<productos.size(); i++)
    if(productos[i] == Fertilizante)
      d.sacarProducto(Fertilizante);
}

vector<Producto> Sistema::productosUsables(Drone d, EstadoCultivo e)
{
  vector<Producto> result;
  for(unsigned int i = 0; i < d.productosDisponibles().size(); i++)
  {
    if(sePuedeAplicarP(d.productosDisponibles()[i], e, d))
      result.push_back(d.productosDisponibles()[i]);
  }
  return result;
}

bool Sistema::sePuedeAplicarP(Producto p, EstadoCultivo e, Drone d)
{
  if((e == RecienSembrado || e == EnCrecimiento) && p == Fertilizante)
  {
    if(d.bateria() > cuantoConsumeP(Fertilizante))
      return true;
  }
  else if(e == ConMaleza)
  {
    if(p == Herbicida && (d.bateria() > cuantoConsumeP(Herbicida)))
      return true;
    else if(p == HerbicidaLargoAlcance && (d.bateria() > cuantoConsumeP(HerbicidaLargoAlcance)))
      return true;
  }
  else if(e == ConPlaga)
  {
    if(p == Plaguicida && (d.bateria() > cuantoConsumeP(Plaguicida)))
      return true;
    else if(p == PlaguicidaBajoConsumo && (d.bateria() > cuantoConsumeP(PlaguicidaBajoConsumo)))
      return true;
  }
  return false;
}

int Sistema::cuantoConsumeP(Producto p)
{
  if(p != Fertilizante)
  {
    if(p == Plaguicida)
      return 10;
    else
      return 5;
  }
  return 0;
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
