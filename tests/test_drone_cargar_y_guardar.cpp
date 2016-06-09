#include <fstream>
#include <algorithm>
#include <drone.h>
#include "gtest/gtest.h"
#include "factories.h"

TEST(test_drone_lo_basico, guardar_y_cargar_funcionan_ok) {
    Drone d1(1, algunos_productos());
    d1.cambiarPosicionActual({1, 3});

    char fname[] = "test_drone.txt";
    std::ofstream out;
    out.open(fname);
    d1.guardar(out);
    Drone d2;
    out.close();
    std::ifstream in(fname);
    d2.cargar(in);
    in.close();

    EXPECT_EQ(d2, d1);
}

TEST(test_drone_lo_basico, guardar_y_cargar_trayectoria_funciona_ok) {
    Drone d1(1, algunos_productos());

    char fname[] = "test_drone.txt";
    std::ofstream out;
    out.open(fname);
    d1.moverA({1, 3});
    d1.moverA({2, 3});
    d1.moverA({3, 3});
    d1.guardar(out);
    Drone d2;
    out.close();
    std::ifstream in(fname);
    d2.cargar(in);
    in.close();

    EXPECT_EQ(d2, d1);
}