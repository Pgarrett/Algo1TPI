#ifndef CAMPO_H_INCLUDED
#define CAMPO_H_INCLUDED

#include<string.h>

#include "tipos.h"

using namespace std;

class Campo
{
  public:
    Campo();
    Campo(const Posicion &posG, const Posicion &posC);

    Dimension dimensiones() const;
    Parcela contenido(const Posicion &pos) const;

    void mostrar(std::ostream& os) const;
    void guardar(std::ostream& os) const;
    void cargar (std::istream& is);

    bool operator==(const Campo& otroCampo) const;

  private:
    Dimension _dimension;
    Grilla<Parcela> _grilla;
};

std::ostream & operator<<(std::ostream & os, const Campo & c);
std::ostream & operator<<(std::ostream & os, const Parcela & p);
std::ostream & operator<<(std::ostream & os, const Producto & p);
std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e);

Parcela tipoDeParcela(string s);
Producto tipoDeProducto(string s);
EstadoCultivo estadoCultivo(string s);
#endif // CAMPO_H_INCLUDED
