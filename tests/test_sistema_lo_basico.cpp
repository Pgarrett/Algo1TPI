#include <sistema.h>
#include "gtest/gtest.h"
#include "factories.h"
#include "auxiliares.h"

TEST(test_sistema_lo_basico, crear_sistema_y_sus_observadores_funcionan_ok) {
    Posicion posC = {5, 15};
    Posicion posG = {10, 20};
    Campo el_campo(posG, posC);
    Secuencia<Drone> ds = algunos_drones_en_granero(posG);
    Sistema s1(el_campo, ds);

    EXPECT_EQ(el_campo, s1.campo());
    for (int i = 0; i < s1.campo().dimensiones().ancho; i++) {
        for (int j = 0; j < s1.campo().dimensiones().largo; j++) {
            if(!(i == posG.x && j == posG.y) && !(i == posC.x && j == posC.y)) {
                EXPECT_EQ(NoSensado, s1.estadoDelCultivo({i, j}));
            }
        }
    }
    EXPECT_TRUE(test::mismos(s1.enjambreDrones(), ds));
}

TEST(test_sistema_lo_basico, crecer_funciona_ok) {
    Posicion posG{0, 0};
    Campo campo_3x3({0, 0}, {0, 1}, {3, 3});
    Secuencia<Drone> ds = algunos_drones_en_granero(posG);
    Sistema s1(campo_3x3, ds);

    s1._estado.parcelas[0][2] = RecienSembrado;
    s1._estado.parcelas[1][0] = NoSensado;
    s1._estado.parcelas[1][1] = RecienSembrado;
    s1._estado.parcelas[1][2] = EnCrecimiento;
    s1._estado.parcelas[2][0] = ConMaleza;
    s1._estado.parcelas[2][1] = ListoParaCosechar;
    s1._estado.parcelas[2][2] = EnCrecimiento;

    s1.crecer();

    EXPECT_EQ(EnCrecimiento, s1.estadoDelCultivo({0, 2}));
    EXPECT_EQ(NoSensado, s1.estadoDelCultivo({1, 0}));
    EXPECT_EQ(EnCrecimiento, s1.estadoDelCultivo({1, 1}));
    EXPECT_EQ(ListoParaCosechar, s1.estadoDelCultivo({1, 2}));
    EXPECT_EQ(ConMaleza, s1.estadoDelCultivo({2, 0}));
    EXPECT_EQ(ListoParaCosechar, s1.estadoDelCultivo({2, 1}));
    EXPECT_EQ(ListoParaCosechar, s1.estadoDelCultivo({2, 2}));

    EXPECT_TRUE(test::mismos(s1.enjambreDrones(), ds));
}
