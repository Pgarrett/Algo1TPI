#include <vector>
#include <drone.h>
#include "gtest/gtest.h"

TEST(test_drone_vuelo_escalerado, siempre_ascendente_es_escalerado) {
    std::vector<Producto> ps;
    Drone d1(1, ps);
    d1.cambiarPosicionActual({4, 4});
    d1.moverA({4, 5});
    d1.moverA({3, 5});
    d1.moverA({3, 6});
    d1.moverA({2, 6});
    EXPECT_TRUE(d1.vueloEscalerado());
}

TEST(test_drone_vuelo_escalerado, siempre_descendente_es_escalerado) {
    std::vector<Producto> ps;
    Drone d1(1, ps);
    d1.moverA({4, 5});
    d1.moverA({5, 5});
    d1.moverA({5, 6});
    d1.moverA({6, 6});
    EXPECT_TRUE(d1.vueloEscalerado());
}

TEST(test_drone_vuelo_escalerado, recto_no_es_escalerado) {
    std::vector<Producto> ps;
    Drone d1(1, ps);
    d1.moverA({4, 5});
    d1.moverA({4, 6});
    d1.moverA({4, 7});
    EXPECT_FALSE(d1.vueloEscalerado());
}
