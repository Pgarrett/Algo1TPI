#include "drone.h"

Drone::Drone()
{
}

Drone::Drone(ID i, const std::vector<Producto>& ps)
{
  _id = i;
  _bateria = 100;
  _enVuelo = false;
  _productos = ps;
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
}

void Drone::guardar(std::ostream & os) const
{
}

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
}

bool Drone::operator==(const Drone & otroDrone) const
{
  return false;
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
  return os;
}
