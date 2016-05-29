#ifndef SISTEMA_H_INCLUDED
#define SISTEMA_H_INCLUDED

#include "tipos.h"
#include "campo.h"
#include "drone.h"

class Sistema
{
  public:
    Sistema();
    Sistema(const Campo &c, const Secuencia<Drone> &ds);

    const Campo& campo() const;
    EstadoCultivo estadoDelCultivo(const Posicion& p) const;
    const Secuencia<Drone>&	enjambreDrones() const;

    void crecer();
    void seVinoLaMaleza(const Secuencia<Posicion>& ps);
    void seExpandePlaga();
    void despegar(const Drone& d);
    bool listoParaCosechar() const;
    void aterrizarYCargarBaterias(Carga b);
    void fertilizarPorFilas();
    void volarYSensar(const Drone& d);

    void mostrar(std::ostream& os) const;
    void guardar(std::ostream& os) const;
    void cargar (std::istream& is);

    bool operator==(const Sistema& otroSistema) const;

  private:
    Campo _campo;
    Secuencia<Drone> _enjambre;
    Grilla<EstadoCultivo> _estado;

    // Auxiliares
    bool enRango(int x, int y) const;
    bool enRango(const Posicion p) const;
    bool enRangoConPlaga(int x, int y) const;
    Posicion posicionGranero() const;
    bool enRangoCultivable(int x, int y) const;
    bool enRangoCultivableLibre(int x, int y) const;
    bool parcelaLibre(int x, int y) const;
    static Posicion vecinoAlOeste(const Posicion &p);

    static bool tieneUnProducto(const Secuencia<Producto> &ps, const Producto &productoABuscar);
    // solución --> deberíamos agregarla como funciones globales en tipos.h
    void split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);
};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os,const Sistema & s);

#endif // SISTEMA_H_INCLUDED
