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

Secuencia<Drone> algunos_drones_en_granero(Posicion posG) {
    Drone d1(1, algunos_productos());
    Drone d2(2, otros_productos());
    d1.cambiarPosicionActual(posG);
    d2.cambiarPosicionActual(posG);
    return {d1, d2};
}