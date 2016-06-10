#include <sistema.h>
#include "gtest/gtest.h"
#include "factories.h"
#include "auxiliares.h"

Drone encontrarDrone(Drone &elDrone, Sistema &s1) {
    for (auto d : s1.enjambreDrones())
        if (d.id() == elDrone.id()) 
            return d;
    return Drone();
}

TEST(test_sistema_aterrizar_y_cargar_baterias, los_drones_con_menor_bateria_deberian_aterrizar_y_cargarse) {
    Posicion posG = {0, 0};
    Campo el_campo(posG, {2, 2});
    Drone elDrone = un_drone();
    elDrone.cambiarPosicionActual({1, 1});
    elDrone.setBateria(60);
    Sistema s1(el_campo, {elDrone});


    s1.aterrizarYCargarBaterias(70);


    EXPECT_EQ(el_campo, s1.campo());
    EXPECT_EQ(1, s1.enjambreDrones().size());
    Drone elDroneEnElSistema = encontrarDrone(elDrone, s1);
    EXPECT_EQ(posG, elDroneEnElSistema.posicionActual());
    EXPECT_EQ(100, elDroneEnElSistema.bateria());
    EXPECT_FALSE(elDroneEnElSistema.enVuelo());
    EXPECT_EQ(elDrone.id(), elDroneEnElSistema.id());
    EXPECT_TRUE(elDroneEnElSistema.vueloRealizado().empty());
}

TEST(test_sistema_aterrizar_y_cargar_baterias, los_drones_con_igual_bateria_deberian_quedarse_como_estan) {
    Campo el_campo({0, 0}, {2, 2});
    Drone elDrone = un_drone();
    elDrone.cambiarPosicionActual({1, 1});
    elDrone.moverA({1, 2});
    elDrone.setBateria(60);
    Sistema s1(el_campo, {elDrone});

    s1.aterrizarYCargarBaterias(60);

    EXPECT_EQ(el_campo, s1.campo());
    EXPECT_EQ(1, s1.enjambreDrones().size());
    
    Drone elDroneEnElSistema = encontrarDrone(elDrone, s1);
    EXPECT_EQ(elDrone.posicionActual(), elDroneEnElSistema.posicionActual());
    EXPECT_EQ(60, elDroneEnElSistema.bateria());
    EXPECT_TRUE(elDroneEnElSistema.enVuelo());
    EXPECT_EQ(elDrone.id(), elDroneEnElSistema.id());
    EXPECT_FALSE(elDroneEnElSistema.vueloRealizado().empty());
}

TEST(test_sistema_aterrizar_y_cargar_baterias, los_drones_con_mayor_bateria_deberian_quedarse_como_estan) {
    Campo el_campo({0, 0}, {2, 2});
    Drone elDrone = un_drone();
    elDrone.cambiarPosicionActual({1, 1});
    elDrone.moverA({1, 2});
    elDrone.setBateria(80);
    Sistema s1(el_campo, {elDrone});

    s1.aterrizarYCargarBaterias(60);

    EXPECT_EQ(el_campo, s1.campo());
    EXPECT_EQ(1, s1.enjambreDrones().size());
    
    Drone elDroneEnElSistema = encontrarDrone(elDrone, s1);
    EXPECT_EQ(elDrone.posicionActual(), elDroneEnElSistema.posicionActual());
    EXPECT_EQ(80, elDroneEnElSistema.bateria());
    EXPECT_TRUE(elDroneEnElSistema.enVuelo());
    EXPECT_EQ(elDrone.id(), elDroneEnElSistema.id());
    EXPECT_FALSE(elDroneEnElSistema.vueloRealizado().empty());
}
