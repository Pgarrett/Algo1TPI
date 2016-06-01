#include<iostream>

#include <string.h>
#include "tipos.h"

using namespace std;

const Posicion operator-(Posicion p1, const Posicion & p2)
{
  p1.x -= p2.x;
  p1.y -= p2.y;
  return p1;
}

std::ostream & operator<<(std::ostream &os, const Parcela &p)
{
  if(p == Casa) os << "Casa";
  if(p == Granero) os << "Granero";
  if(p == Cultivo) os << "Cultivo";
	return os;
}

std::ostream & operator<<(std::ostream & os, const Posicion & p)
{
  os << "[" << p.x << "," << p.y << "]";
  return os;
}

std::ostream & operator<<(std::ostream &os, const Producto &p)
{
  if(p == Fertilizante) os << "Fertilizante";
  if(p == Plaguicida) os << "Plaguicida";
  if(p == PlaguicidaBajoConsumo) os << "PlaguicidaBajoConsumo";
  if(p == Herbicida) os << "Herbicida";
  if(p == HerbicidaLargoAlcance) os << "HerbicidaLargoAlcance";
	return os;
}

std::ostream & operator<<(std::ostream &os, const EstadoCultivo &e)
{
  if(e == RecienSembrado) os << "RecienSembrado";
  if(e == EnCrecimiento) os << "EnCrecimiento";
  if(e == ListoParaCosechar) os << "ListoParaCosechar";
  if(e == ConMaleza) os << "ConMaleza";
  if(e == ConPlaga) os << "ConPlaga";
  if(e == NoSensado) os << "NoSensado";
	return os;
}

const bool operator==(const Posicion &p1, const Posicion &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

const bool operator==(const InfoVueloCruzado &a, const InfoVueloCruzado &b) {
    return a.posicion == b.posicion && a.cantidadCruces == b.cantidadCruces;
}

const bool operator<(InfoVueloCruzado i1, InfoVueloCruzado i2) {
  return i1.cantidadCruces < i2.cantidadCruces;
}

Parcela tipoDeParcela(string s)
{
  if(s == "Casa") return Casa;
  if(s == "Granero") return Granero;
  if(s == "Cultivo") return Cultivo;
}

Producto tipoDeProducto(string s)
{
  if(s == "Fertilizante") return Fertilizante;
  if(s == "Plaguicida") return Plaguicida;
  if(s == "PlaguicidaBajoConsumo") return PlaguicidaBajoConsumo;
  if(s == "Herbicida") return Herbicida;
  if(s == "HerbicidaLargoAlcance") return HerbicidaLargoAlcance;
}

EstadoCultivo estadoCultivo(string s)
{
  if(s == "RecienSembrado") return RecienSembrado;
  if(s == "EnCrecimiento") return EnCrecimiento;
  if(s == "ListoParaCosechar") return ListoParaCosechar;
  if(s == "ConMaleza") return ConMaleza;
  if(s == "ConPlaga") return ConPlaga;
  if(s == "NoSensado") return NoSensado;
}
