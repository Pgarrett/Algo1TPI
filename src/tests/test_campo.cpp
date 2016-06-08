#include <fstream>
#include "gtest/gtest.h"
#include "factories.h"

bool alcanzableEn100(Campo c, Posicion posG) {
    for (int i = 0; i < c.dimensiones().ancho; ++i) {
        for (int j = 0; j < c.dimensiones().largo; ++j) {
            if (abs(posG.x - i) + abs(posG.y - j) > 100) {
                return false;
            }
        }
    }
    return true;
}

TEST(test_campo, crear_campo_y_sus_observadores_funcionan_ok) {
    Posicion posG {5, 10};
    Campo c1(posG, {8, 12});
    EXPECT_GT(c1.dimensiones().ancho, 8);
    EXPECT_GT(c1.dimensiones().largo, 12);
    EXPECT_EQ(Granero, c1.contenido({5, 10}));
    EXPECT_EQ(Casa, c1.contenido({8, 12}));
    EXPECT_TRUE(alcanzableEn100(c1, posG));
}

TEST(test_campo, guardar_y_cargar_campo_funcionan_ok) {
    Campo c1 = un_campo();

    char fname[] = "test_campo.txt";
    std::ofstream out(fname);
    c1.guardar(out);

    out.close();
    Campo c2;
    std::ifstream in(fname);
    c2.cargar(in);
    in.close();

    EXPECT_EQ(c1, c2);
}