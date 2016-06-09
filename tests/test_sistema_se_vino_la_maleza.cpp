#include <sistema.h>
#include "gtest/gtest.h"
#include "factories.h"
#include "auxiliares.h"

TEST(test_sistema_se_vino_la_maleza, lista_vacia_no_modifica_nada) {
    Posicion posG{0, 0};
    Campo campo_3x3(posG, {0, 1}, {3, 3});
    Secuencia<Drone> ds = algunos_drones_en_granero(posG);
    Sistema s1(campo_3x3, ds);

    s1._estado.parcelas[0][2] = RecienSembrado;
    s1._estado.parcelas[1][0] = NoSensado;
    s1._estado.parcelas[1][1] = RecienSembrado;
    s1._estado.parcelas[1][2] = EnCrecimiento;
    s1._estado.parcelas[2][0] = ConMaleza;
    s1._estado.parcelas[2][1] = ListoParaCosechar;
    s1._estado.parcelas[2][2] = EnCrecimiento;

    s1.seVinoLaMaleza({});

    EXPECT_EQ(RecienSembrado, s1.estadoDelCultivo({0, 2}));
    EXPECT_EQ(NoSensado, s1.estadoDelCultivo({1, 0}));
    EXPECT_EQ(RecienSembrado, s1.estadoDelCultivo({1, 1}));
    EXPECT_EQ(EnCrecimiento, s1.estadoDelCultivo({1, 2}));
    EXPECT_EQ(ConMaleza, s1.estadoDelCultivo({2, 0}));
    EXPECT_EQ(ListoParaCosechar, s1.estadoDelCultivo({2, 1}));
    EXPECT_EQ(EnCrecimiento, s1.estadoDelCultivo({2, 2}));

    EXPECT_EQ(campo_3x3, s1.campo());
    EXPECT_TRUE(test::mismos(s1.enjambreDrones(), ds));
}

TEST(test_sistema_se_vino_la_maleza, se_pone_con_maleza_las_posiciones_de_la_lista) {
    Posicion posG{0, 0};
    Campo campo_3x3(posG, {0, 1}, {3, 3});
    Secuencia<Drone> ds = algunos_drones_en_granero(posG);
    Sistema s1(campo_3x3, ds);

    s1._estado.parcelas[0][2] = RecienSembrado;
    s1._estado.parcelas[1][0] = NoSensado;
    s1._estado.parcelas[1][1] = RecienSembrado;
    s1._estado.parcelas[1][2] = EnCrecimiento;
    s1._estado.parcelas[2][0] = ConMaleza;
    s1._estado.parcelas[2][1] = ListoParaCosechar;
    s1._estado.parcelas[2][2] = EnCrecimiento;

    s1.seVinoLaMaleza({{0, 2},
                       {1, 2}});

    EXPECT_EQ(ConMaleza, s1.estadoDelCultivo({0, 2}));
    EXPECT_EQ(NoSensado, s1.estadoDelCultivo({1, 0}));
    EXPECT_EQ(RecienSembrado, s1.estadoDelCultivo({1, 1}));
    EXPECT_EQ(ConMaleza, s1.estadoDelCultivo({1, 2}));
    EXPECT_EQ(ConMaleza, s1.estadoDelCultivo({2, 0}));
    EXPECT_EQ(ListoParaCosechar, s1.estadoDelCultivo({2, 1}));
    EXPECT_EQ(EnCrecimiento, s1.estadoDelCultivo({2, 2}));

    EXPECT_EQ(campo_3x3, s1.campo());
    EXPECT_TRUE(test::mismos(s1.enjambreDrones(), ds));
}
