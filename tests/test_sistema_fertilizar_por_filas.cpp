#include <sistema.h>
#include "gtest/gtest.h"
#include "factories.h"
#include "auxiliares.h"

Drone buscarDronesito(Drone &elDrone, Sistema &s1) {
    for (auto d : s1.enjambreDrones())
        if (d.id() == elDrone.id()) 
            return d;
    return Drone();
}//TODO sacar a una auxiliar

TEST(test_sistema_fertilizar_por_filas, el_campo_no_debe_modificarse) {
    Campo el_campo({0, 0}, {2, 2});
    Drone elDrone = Drone(1, algunos_productos());
    elDrone.cambiarPosicionActual({1, 1});
    Sistema s1(el_campo, {elDrone});

    s1.fertilizarPorFilas();

    EXPECT_EQ(el_campo, s1.campo());
}

TEST(test_sistema_fertilizar_por_filas, debe_quedar_la_misma_cantidad_de_drones) {
    Campo el_campo({0, 0}, {2, 2});
    Drone elDrone = Drone(1, algunos_productos());
    elDrone.cambiarPosicionActual({1, 1});
    Sistema s1(el_campo, {elDrone});

    s1.fertilizarPorFilas();

    EXPECT_EQ(1, s1.enjambreDrones().size());
}

TEST(test_sistema_fertilizar_por_filas, los_drones_en_el_granero_no_deben_modificarse) {
    Posicion posG = {0, 0};
    Campo el_campo(posG, {2, 2});
    Secuencia<Drone> ds = algunos_drones_en_granero(posG);
    Sistema s1(el_campo, ds);

    s1.fertilizarPorFilas();

    for(Drone d : ds) {
        Drone droneEnElSistema = buscarDronesito(d, s1);
        EXPECT_EQ(d, droneEnElSistema);
    }
}

TEST(test_sistema_fertilizar_por_filas, fertiliza_hasta_el_final_ok) {
    Posicion posG = {1, 1};
    Campo el_campo(posG, {2, 2});
    Drone d(1, {Fertilizante, Fertilizante});
    Secuencia<Drone> ds = {d};
    Sistema s1(el_campo, ds);
    
    Drone & d1 = s1._enjambre[0];
    d1.moverA({1, 0});
    s1._estado.parcelas[0][0] = EnCrecimiento;
    s1._estado.parcelas[1][0] = RecienSembrado;

    s1.fertilizarPorFilas();

    EXPECT_EQ(d1.posicionActual(), Posicion(0,0));
    EXPECT_EQ(d1.productosDisponibles().size(), 0);
    EXPECT_EQ(s1.estadoDelCultivo({0,0}), ListoParaCosechar);
    EXPECT_EQ(s1.estadoDelCultivo({1,0}), ListoParaCosechar);
}

TEST(test_sistema_fertilizar_por_filas, fertiliza_hasta_la_casa_ok) {
    Posicion posG = {3, 0};
    Campo el_campo(posG, {0, 0});
    Drone d(1, {Fertilizante, Fertilizante, Fertilizante});
    Secuencia<Drone> ds = {d};
    Sistema s1(el_campo, ds);
    
    Drone & d1 = s1._enjambre[0];
    d1.moverA({2, 0});

    s1.fertilizarPorFilas();

    EXPECT_EQ(d1.posicionActual(), Posicion(0,0));
    EXPECT_EQ(d1.productosDisponibles().size(), 1);
}

TEST(test_sistema_fertilizar_por_filas, fertiliza_hasta_sin_bateria_ok) {
    Posicion posG = {4, 0};
    Campo el_campo(posG, {0, 1});
    Drone d(1, {Fertilizante, Fertilizante});
    Secuencia<Drone> ds = {d};
    Sistema s1(el_campo, ds);
    
    Drone & d1 = s1._enjambre[0];
    d1.moverA({3, 0});
    d1.setBateria(1);

    s1.fertilizarPorFilas();

    EXPECT_EQ(d1.posicionActual(), Posicion(2,0));
    EXPECT_EQ(d1.productosDisponibles().size(), 0);
}

TEST(test_sistema_fertilizar_por_filas, fertiliza_hasta_sin_fertilizante_ok) {
    Posicion posG = {3, 0};
    Campo el_campo(posG, {0, 1});
    Drone d(1, {Fertilizante});
    Secuencia<Drone> ds = {d};
    Sistema s1(el_campo, ds);
    
    Drone & d1 = s1._enjambre[0];
    d1.moverA({2, 0});

    s1.fertilizarPorFilas();

    EXPECT_EQ(d1.posicionActual(), Posicion(2,0));
    EXPECT_EQ(d1.productosDisponibles().size(), 0);
}

/*TEST(test_sistema_fertilizar_por_filas, los_drones_en_vuelo_deben_hacer_su_recorrido_maximo_a_la_izquierda) {
    Posicion posG = {0, 0};
    Campo el_campo(posG, {2, 2});
    Drone elDrone = Drone(1, algunos_productos());
    elDrone.cambiarPosicionActual({1, 1});

    Sistema s1(el_campo, {elDrone});

    s1.fertilizarPorFilas();

     
}*/

//TEST(test_sistema_fertilizar_por_filas, las_parcelas_a_la_derecha_de_un_drone_no_deben_modificarse) { }

//TEST(test_sistema_fertilizar_por_filas, las_parcelas_a_fuera_del_recorrido_maximo_deben_quedar_igual) { }

//TEST(test_sistema_fertilizar_por_filas, los_cultivos_que_no_son_recien_sembrados_o_en_crecimiento_quedan_igual) { }

//TEST(test_sistema_fertilizar_por_filas, los_cultivos_que_son_recien_sembrados_o_en_crecimiento_pasan_a_listo_para_cosechar) { }

