#include "drone.h"
#include "auxiliares.h"

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

  Posicion delta = _trayectoria[2]-_trayectoria[0];
  if(abs(delta.x) != 1 || abs(delta.y) != 1)
    return false;

  Posicion delta0 = _trayectoria[1]-_trayectoria[0];
  Posicion delta1 = _trayectoria[2]-_trayectoria[1];

  for(unsigned int i=0; i<_trayectoria.size()-2; i++)
  {
    if((i%2 == 0 && _trayectoria[i+1]-_trayectoria[i] != delta0)
      || (i%2 == 1 && _trayectoria[i+1]-_trayectoria[i] != delta1))
       return false;
  }
  return true;
}

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds)
{
  return Secuencia<InfoVueloCruzado>();
}

void Drone::mostrar(std::ostream & os) const
{
  os << "{ D " << _id << " " << _bateria << " }";
}

// { D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]}
void Drone::guardar(std::ostream & os) const
{
  os << "{ D " << _id << " " << _bateria << " ";

  // Trayectoria
  os << "[";
  for(unsigned int i=0; i<_trayectoria.size(); i++)
  {
    os << "[" << _trayectoria[i].x << "," << _trayectoria[i].y;

    if(i < _trayectoria.size()-1)
      os << "],";
    else
      os << "]";
  }
  os << "] ";

  // Productos
  os << "[";
  for(unsigned int i=0; i<_productos.size(); i++)
  {
    os << _productos[i];

    if(i < _productos.size()-1)
      os << ", ";
  }
  os << "]}";
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
