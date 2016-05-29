#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <string.h>
#include "tipos.h"

using namespace std;

std::ostream & operator<<(std::ostream & os, const Parcela & p);
std::ostream & operator<<(std::ostream & os, const Producto & p);
std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e);

Parcela tipoDeParcela(string s);
Producto tipoDeProducto(string s);
EstadoCultivo estadoCultivo(string s);

#endif
