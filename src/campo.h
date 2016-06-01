#ifndef CAMPO_H_INCLUDED
#define CAMPO_H_INCLUDED

#include "tipos.h"

using namespace std;

class Campo
{
  public:
    Campo();
    Campo(const Posicion &posG, const Posicion &posC, Dimension dimension);
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
#endif // CAMPO_H_INCLUDED
