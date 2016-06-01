#include <tipos.h>
#include <campo.h>
#include <drone.h>

Campo un_campo() {
    return Campo({5, 15}, {10, 20});
}

std::vector<Producto> algunos_productos() {
    return {HerbicidaLargoAlcance, PlaguicidaBajoConsumo, Herbicida};
}

std::vector<Producto> otros_productos() {
    return {Fertilizante, PlaguicidaBajoConsumo, Herbicida};
}

std::vector<Producto> algunos_productos_pero_en_otro_orden() {
    return {HerbicidaLargoAlcance, Herbicida, PlaguicidaBajoConsumo};
}

Drone un_drone() {
    return Drone(1, algunos_productos());
}

Secuencia<Drone> algunos_drones() {
    Drone d1(1, algunos_productos());
    Drone d2(1, otros_productos());
    return {d1, d2};
}