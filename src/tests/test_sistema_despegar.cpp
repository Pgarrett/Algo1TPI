#include <sistema.h>
#include "gtest/gtest.h"
#include "factories.h"
#include "auxiliares.h"

Drone buscarDroneEnSistema(Drone &elDrone, Sistema &s1) {
    Drone elDroneEnElSistema;
    for (int i = 0; i < s1.enjambreDrones().size(); i++) {
        if (s1.enjambreDrones()[i].id() == elDrone.id()) {
            elDroneEnElSistema = s1.enjambreDrones()[i];
            break;
        }
    }
    return elDroneEnElSistema;
}

TEST(test_sistema_despegar, si_todas_estan_libres_comienza_vuelo_en_alguna_al_lado_del_granero_en_rango) {
    Posicion posG = {0, 0};

    Drone elDrone = un_drone();
    elDrone.cambiarPosicionActual(posG);

    Campo el_campo(posG, {2, 2});
    Sistema s1(el_campo, {elDrone});

    s1.despegar(elDrone);
    Drone elDroneEnElSistema = buscarDroneEnSistema(elDrone, s1);

    EXPECT_TRUE(elDroneEnElSistema.enVuelo());

    Posicion aLaDerechaDelGranero = {0, 1};
    Posicion abajoDelGranero = {1, 0};
    bool estaContiguoAlGranero =
            elDroneEnElSistema.posicionActual() == aLaDerechaDelGranero ||
            elDroneEnElSistema.posicionActual() == abajoDelGranero;
    EXPECT_TRUE(estaContiguoAlGranero);

    EXPECT_EQ(el_campo, s1.campo());
    EXPECT_EQ(1, s1.enjambreDrones().size());
}

TEST(test_sistema_despegar, si_hay_drone_o_casa_al_lado_del_granero_comienza_vuelo_en_una_libre) {
    Posicion posG = {1, 1};

    Drone elDrone = Drone(1, algunos_productos());
    elDrone.cambiarPosicionActual(posG);
    Drone otroDrone = Drone(2, algunos_productos());
    otroDrone.cambiarPosicionActual(posG);

    Posicion posCasaALaDerechaDelGranero = {1, 2};
    Campo el_campo(posG, posCasaALaDerechaDelGranero);

    Sistema s1(el_campo, {elDrone, otroDrone});
    s1.despegar(otroDrone);
    s1.despegar(elDrone);

    Drone elDroneEnElSistema = buscarDroneEnSistema(elDrone, s1);
    Drone elOtroDroneEnElSistema = buscarDroneEnSistema(otroDrone, s1);

    EXPECT_TRUE(elDroneEnElSistema.enVuelo());
    EXPECT_TRUE(elOtroDroneEnElSistema.enVuelo());

    bool estanEnDiferentePosicion = not(elDroneEnElSistema.posicionActual() == elOtroDroneEnElSistema.posicionActual());
    EXPECT_TRUE(estanEnDiferentePosicion);

    Posicion arribaDelGranero = {0, 1};
    Posicion abajoDelGranero = {2, 1};
    Posicion izquierdaDelGranero = {1,0};
    bool estaContiguoAlGranero =
            elDroneEnElSistema.posicionActual() == arribaDelGranero ||
            elDroneEnElSistema.posicionActual() == abajoDelGranero ||
            elDroneEnElSistema.posicionActual() == izquierdaDelGranero;
    EXPECT_TRUE(estaContiguoAlGranero);

    EXPECT_EQ(el_campo, s1.campo());
    EXPECT_EQ(2, s1.enjambreDrones().size());
}



