#include "campo.h"

Campo::Campo()
{
}

Campo::Campo(const Posicion &posG, const Posicion &posC)
{
}

Dimension Campo::dimensiones() const
{
  return _dimension;
}

Parcela Campo::contenido(const Posicion & p) const
{
  return _grilla.parcelas[p.x][p.y];
}

void Campo::mostrar(std::ostream & os) const
{
}

void Campo::guardar(std::ostream & os) const
{
}

void Campo::cargar(std::istream & is)
{
}

bool Campo::operator==(const Campo & otroCampo) const
{
  return false;
}

std::ostream & operator<<(std::ostream & os, const Campo & c)
{
  return os;
}

std::ostream & operator<<(std::ostream & os, const Parcela & p)
{
  return os;
}

std::ostream & operator<<(std::ostream & os, const Producto & p)
{
  return os;
}

std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e)
{
  return os;
}
