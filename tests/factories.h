#ifndef TESTS_FACTORIES_H
#define TESTS_FACTORIES_H

#include <campo.h>
#include <drone.h>

Campo un_campo();

std::vector<Producto> algunos_productos();

std::vector<Producto> otros_productos();

std::vector<Producto> algunos_productos_pero_en_otro_orden();

Drone un_drone();

Secuencia<Drone> algunos_drones_en_granero(Posicion posG);

#endif //TESTS_FACTORIES_H
