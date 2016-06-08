#include <fstream>
#include <sistema.h>
#include "gtest/gtest.h"
#include "factories.h"

TEST(test_sistema, guardar_y_cargar_sistema_funcionan_ok) {
    Posicion posG{5, 15};
    Sistema s1(Campo(posG, {10, 20}), algunos_drones_en_granero(posG));

    char fname[] = "test_sistema.txt";
    std::ofstream out;
    out.open(fname);
    s1.guardar(out);
    out.close();

    Sistema s2;
    std::ifstream in(fname);
    s2.cargar(in);
    in.close();

    EXPECT_EQ(s1, s2);
}

TEST(test_sistema, guardar_y_cargar_sistema_con_un_solo_drone_con_trayectoria_funcionan_ok) {
    Posicion posG{5, 10};
    Drone d1(1,algunos_productos());
    d1.cambiarPosicionActual(posG);

    Secuencia<Drone> ds = Secuencia<Drone>();
    ds.push_back(d1);
    Sistema s1(Campo(posG, {10, 10}), ds);

    s1.volarYSensar(ds[0]);
    s1.volarYSensar(ds[0]);

    char fname[] = "test_sistema.txt";
    std::ofstream out;
    out.open(fname);
    s1.guardar(out);
    out.close();

    Sistema s2;
    std::ifstream in(fname);
    s2.cargar(in);
    in.close();

    EXPECT_EQ(s1, s2);
}


TEST(test_sistema, guardar_y_cargar_sistema_con_drones_con_trayectorias_funcionan_ok) {
    Posicion posG{5, 10};
    Secuencia<Drone> ds = algunos_drones_en_granero(posG);
    Sistema s1(Campo(posG, {20, 20}), ds);

    s1.volarYSensar(ds[0]);
    s1.volarYSensar(ds[0]);
    s1.volarYSensar(ds[1]);
    s1.volarYSensar(ds[1]);

    char fname[] = "test_sistema.txt";
    std::ofstream out;
    out.open(fname);
    s1.guardar(out);
    out.close();

    Sistema s2;
    std::ifstream in(fname);
    s2.cargar(in);
    in.close();

    EXPECT_EQ(s1, s2);
}
