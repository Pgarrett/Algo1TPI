#ifndef DRONE_H_INCLUDED
#define DRONE_H_INCLUDED

#include "tipos.h"
#include <vector>

class Drone{
    public:
        Drone();
        Drone(ID i, const std::vector<Producto> &ps);

        ID id() const;
        Carga bateria() const;
        bool enVuelo() const;
        const Secuencia<Posicion>& vueloRealizado() const;
        Posicion posicionActual() const;
		const Secuencia<Producto>& productosDisponibles() const;

        bool vueloEscalerado() const;
		static Secuencia<InfoVueloCruzado>	vuelosCruzados(const Secuencia<Drone>& ds);

		void mostrar(std::ostream& os) const;
		void guardar(std::ostream& os) const;
		void cargar(std::istream& is);

        bool operator==(const Drone& otroDrone) const;

	private:
		ID _id;
		Carga _bateria;
		Secuencia<Posicion> _trayectoria;
		Secuencia<Producto> _productos;
		bool _enVuelo;
};

// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream & operator<<(std::ostream & os,const Drone & d);

#endif // DRONE_H_INCLUDED
