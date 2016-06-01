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
std::ostream &operator<<(std::ostream &os, const Parcela &p);
Parcela tipoDeParcela(string s);

enum Producto {Fertilizante, Plaguicida, PlaguicidaBajoConsumo, Herbicida, HerbicidaLargoAlcance};
std::ostream &operator<<(std::ostream &os, const Producto &p);
Producto tipoDeProducto(string s);

enum EstadoCultivo {RecienSembrado, EnCrecimiento, ListoParaCosechar, ConMaleza, ConPlaga, NoSensado};
std::ostream &operator<<(std::ostream &os, const EstadoCultivo &e);
EstadoCultivo estadoCultivo(string s);

struct Posicion
{
  int x;
  int y;

  inline Posicion() { }
  inline Posicion(int px, int py) { x = px; y = py; }
};
const Posicion operator-(Posicion p1, const Posicion & p2);
const bool operator==(const Posicion &p1, const Posicion &p2);
std::ostream & operator<<(std::ostream & os, const Posicion & p);

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
const bool operator==(const InfoVueloCruzado &a, const InfoVueloCruzado &b);
const bool operator<(InfoVueloCruzado i1, InfoVueloCruzado i2);

#endif /*TIPOS_H*/
