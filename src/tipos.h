#ifndef TIPOS_H
#define TIPOS_H

#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

typedef int ID;
typedef int Carga;
typedef int Ancho;
typedef int Largo;

#define Secuencia std::vector

enum Parcela {Cultivo, Granero, Casa};
enum Producto {Fertilizante, Plaguicida, PlaguicidaBajoConsumo, Herbicida, HerbicidaLargoAlcance};
enum EstadoCultivo {RecienSembrado, EnCrecimiento, ListoParaCosechar, ConMaleza, ConPlaga, NoSensado};

struct Posicion
{
  int x;
  int y;

  inline bool operator==(Posicion p) const { return p.x == x && p.y == y; }
  inline bool operator!=(Posicion p) const { return p.x != x || p.y != y; }
};
Posicion operator-(Posicion p1, const Posicion & p2);

struct Dimension
{
  Ancho ancho;
  Largo largo;
};

template<class T>
struct Grilla
{
  Grilla() {}
  Grilla(Dimension d)
  {
    parcelas.resize(d.ancho, std::vector<T>(d.largo, static_cast<T>(0)));
  }
  std::vector< std::vector<T> > parcelas;
};

struct InfoVueloCruzado
{
  Posicion posicion;
  int cantidadCruces;
};

std::ostream & operator<<(std::ostream & os, const Parcela & p);
std::ostream & operator<<(std::ostream & os, const Producto & p);
std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e);

Parcela tipoDeParcela(string s);
Producto tipoDeProducto(string s);
EstadoCultivo estadoCultivo(string s);

#endif /*TIPOS_H*/
