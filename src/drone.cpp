#include "drone.h"
#include "auxiliares.cpp"

using namespace std;

Drone::Drone()
{
}

Drone::Drone(ID i, const std::vector<Producto>& ps)
{
  _id = i;
  _bateria = 100;
  _trayectoria = Secuencia<Posicion>();
  _productos = ps;
  _enVuelo = false;
}

ID Drone::id() const
{
  return _id;
}

Carga Drone::bateria() const
{
  return _bateria;
}

bool Drone::enVuelo() const
{
  return _enVuelo;
}

const Secuencia<Posicion>& Drone::vueloRealizado() const
{
  return _trayectoria;
}

Posicion Drone::posicionActual() const
{
  return _posicionActual;
}

const Secuencia<Producto>& Drone::productosDisponibles() const
{
  return _productos;
}

bool Drone::vueloEscalerado() const
{
  if(_trayectoria.size() < 3)
    return true;

  Posicion primerosTres = _trayectoria[2]-_trayectoria[0];
  if(abs(primerosTres.x) != 1 || abs(primerosTres.y) != 1)
    return false;

  vector<Posicion> delta;
  delta.push_back(_trayectoria[1]-_trayectoria[0]);
  delta.push_back(_trayectoria[2]-_trayectoria[1]);

  unsigned int i = 0;
  bool esEscalera = true;

  // Notemos t = _trayectoria para mejorar la legibilidad

  // Pc: i == 0 && esEscalera && |t| >= 3 && delta[0] == t[1]-t[0] && delta[1] == t[2]-t[1]
  // Qc: i == |t|-1 && (!esEscalera || (esEscalera && todos(t[i+1]-t[i] == delta[i%2], i in [0..t-1))))
  // I: 0 <= i <= |t|-1 && (!esEscalera || (esEscalera && todos(t[j+1]-t[j] == delta[j%2], j in [0..i))))
  // B: i < |t|-1
  // cota: 0
  // fv: |t|-1-i

  // vale Pc: i == 0 && esEscalera && |t| >= 3 && delta[0] == t[1]-t[0] && delta[1] == t[2]-t[1]
  while(i<_trayectoria.size()-1)
  {
    if(_trayectoria[i+1]-_trayectoria[i] != delta[i%2])
       esEscalera = false;
    i++;
  }
  return esEscalera;
}

  // backup Qc: i == |t|-1 && (!esEscalera
  //                || (esEscalera && todos((i%2 == 0 && t[i+1]-t[i] == delta0)
  //                || (i%2 == 1 && t[i+1]-t[i] == delta1), i in [0..t-1)))

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds)
{
  Secuencia<InfoVueloCruzado> res = Secuencia<InfoVueloCruzado>();

  if(ds.size() < 2)
    return res;

  Drone d = ds[0];
  for(int j=0; j<d.vueloRealizado().size(); j++)
    agregarCruces(ds, j, d.vueloRealizado()[j], res);

  ordenar(res);
  return res;
}

void Drone::mostrar(std::ostream & os) const
{
  os << "{ D " << _id << " " << _bateria << " }";
}

// { D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]}
void Drone::guardar(std::ostream & os) const
{
  os << "{ D " << _id << " " << _bateria << " ";

  mostrarSecuencia(os, _trayectoria);
  os << " ";

  mostrarSecuencia(os, _productos);
  os << "}";
}

// TODO [Phil]
void Drone::cargar(std::istream & is)
{
}

void Drone::moverA(const Posicion pos)
{
  _trayectoria.push_back(pos);
}

void Drone::setBateria(const Carga c)
{
  _bateria = c;
}

void Drone::borrarVueloRealizado()
{
  _trayectoria.clear();
  _enVuelo = false;
}

void Drone::cambiarPosicionActual(const Posicion p)
{
  _posicionActual = p;
}

void Drone::sacarProducto(const Producto p)
{
  for(unsigned int i = 0; i < _productos.size(); i++)
  {
    if(p == _productos[i])
    {
      _productos.erase(_productos.begin() + i);
      return;
    }
  }
}

bool Drone::operator==(const Drone & otroDrone) const
{
  if(_id == otroDrone.id()
     && _bateria == otroDrone.bateria()
     && _trayectoria == otroDrone.vueloRealizado()
     && _posicionActual == otroDrone.posicionActual()
     && _enVuelo == otroDrone.enVuelo()
     && mismos(_productos, otroDrone.productosDisponibles()))
      return true;
  return false;
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
  d.mostrar(os);
  return os;
}

void agregarCruce(Posicion p, int cantidad, Secuencia<InfoVueloCruzado> & vs)
{
  for(unsigned int i=0; i<vs.size(); i++)
  {
    if(vs[i].posicion == p)
    {
      vs[i].cantidadCruces += cantidad;
      return;
    }
  }

  InfoVueloCruzado nuevoCruce = InfoVueloCruzado();
  nuevoCruce.posicion = p;
  nuevoCruce.cantidadCruces = cantidad;
  vs.push_back(nuevoCruce);
}

void agregarCruces(Secuencia<Drone> ds, int instante, Posicion p, Secuencia<InfoVueloCruzado> & vs)
{
  int cruces = 0;
  for(unsigned int i=0; i<ds.size(); i++)
    if(ds[i].vueloRealizado()[instante] == p)
      cruces++;

  if(cruces > 1)
    agregarCruce(p, cruces, vs);
}
