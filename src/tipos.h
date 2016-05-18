#ifndef TIPOS_H
#define TIPOS_H

#include <iostream>
#include <vector>

typedef int ID;
typedef int Carga;
typedef int Ancho;
typedef int Largo;

#define Secuencia std::vector

enum Parcela {Cultivo, Granero, Casa};
enum Producto {Fertilizante, Plaguicida, PlaguicidaBajoConsumo, Herbicida, HerbicidaLargoAlcance};
enum EstadoCultivo {RecienSembrado, EnCrecimiento, ListoParaCosechar, ConMaleza, ConPlaga, NoSensado};

struct Posicion {
    int x;
    int y;
};

struct Dimension {
    Ancho ancho;
    Largo largo;
};

template<class T>
struct Grilla {
    Grilla() {}
	Grilla(Dimension d)	{
		parcelas.resize(d.ancho, std::vector<T>(d.largo, Cultivo));
	}
    std::vector< std::vector<T> > parcelas;
};

struct InfoVueloCruzado {
    Posicion posicion;
    int cantidadCruces;
};

#endif /*TIPOS_H*/
