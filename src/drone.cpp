#include "drone.h"

Drone::Drone()
{
}

Drone::Drone(ID i, const std::vector<Producto>& ps)
{
}

ID Drone::id() const
{
	return ID();
}

Carga Drone::bateria() const
{
	return Carga();
}

bool Drone::enVuelo() const
{
	return false;
}

const Secuencia<Posicion>& Drone::vueloRealizado() const
{
	return Secuencia<Posicion>();
}

Posicion Drone::posicionActual() const
{
	return Posicion();
}

const Secuencia<Producto>& Drone::productosDisponibles() const
{
	return Secuencia<Producto>();
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

bool Drone::operator==(const Drone & otroDrone) const
{
	return false;
}

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
	return os;
}
