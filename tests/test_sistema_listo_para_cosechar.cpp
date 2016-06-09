#include <sistema.h>
#include "gtest/gtest.h"
#include "factories.h"

TEST(test_sistema_listo_para_cosechar, si_es_menos_del_90_por_ciento_retorna_false) {
    Posicion posG{0, 0};
    Campo campo_4x3(posG, {0, 1}, {4, 3});
    Sistema s1(campo_4x3, algunos_drones_en_granero(posG));

    s1._estado.parcelas[0][2] = ListoParaCosechar;
    s1._estado.parcelas[1][0] = ListoParaCosechar;
    s1._estado.parcelas[1][1] = ListoParaCosechar;
    s1._estado.parcelas[1][2] = ListoParaCosechar;
    s1._estado.parcelas[2][0] = EnCrecimiento;
    s1._estado.parcelas[2][1] = EnCrecimiento;
    s1._estado.parcelas[2][2] = EnCrecimiento;
    s1._estado.parcelas[3][0] = EnCrecimiento;
    s1._estado.parcelas[3][1] = EnCrecimiento;
    s1._estado.parcelas[3][2] = EnCrecimiento;

    EXPECT_FALSE(s1.listoParaCosechar());
}

TEST(test_sistema_listo_para_cosechar, si_es_exactamente_el_90_por_ciento_retorna_true) {
    Posicion posG{0, 0};
    Campo campo_4x3(posG, {0, 1}, {4, 3});
    Sistema s1(campo_4x3, algunos_drones_en_granero(posG));

    s1._estado.parcelas[0][2] = ListoParaCosechar;
    s1._estado.parcelas[1][0] = ListoParaCosechar;
    s1._estado.parcelas[1][1] = ListoParaCosechar;
    s1._estado.parcelas[1][2] = ListoParaCosechar;
    s1._estado.parcelas[2][0] = ListoParaCosechar;
    s1._estado.parcelas[2][1] = ListoParaCosechar;
    s1._estado.parcelas[2][2] = ListoParaCosechar;
    s1._estado.parcelas[3][0] = ListoParaCosechar;
    s1._estado.parcelas[3][1] = ListoParaCosechar;
    s1._estado.parcelas[3][2] = EnCrecimiento;

    EXPECT_TRUE(s1.listoParaCosechar());
}

TEST(test_sistema_listo_para_cosechar, si_es_mas_del_90_por_ciento_retorna_true) {
    Posicion posG{0, 0};
    Campo campo_4x3(posG, {0, 1}, {4, 3});
    Sistema s1(campo_4x3, algunos_drones_en_granero(posG));

    s1._estado.parcelas[0][2] = ListoParaCosechar;
    s1._estado.parcelas[1][0] = ListoParaCosechar;
    s1._estado.parcelas[1][1] = ListoParaCosechar;
    s1._estado.parcelas[1][2] = ListoParaCosechar;
    s1._estado.parcelas[2][0] = ListoParaCosechar;
    s1._estado.parcelas[2][1] = ListoParaCosechar;
    s1._estado.parcelas[2][2] = ListoParaCosechar;
    s1._estado.parcelas[3][0] = ListoParaCosechar;
    s1._estado.parcelas[3][1] = ListoParaCosechar;
    s1._estado.parcelas[3][2] = ListoParaCosechar;

    EXPECT_TRUE(s1.listoParaCosechar());
}

