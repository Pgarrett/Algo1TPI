#include "drone.h"

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
  return false;
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
  for(int i=0; i<_trayectoria.size(); i++)
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
  for(int i=0; i<_productos.size(); i++)
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
  int pos;
  for(int i = 0; i < _productos.size(); i++)
  {
    if(p == _productos[i]) pos = i; //TODO buscar una manera que no tire warnings
  }
  _productos.erase(_productos.begin() + pos);
}

bool Drone::operator==(const Drone & otroDrone) const
{
  return false;
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
  return os;
}
