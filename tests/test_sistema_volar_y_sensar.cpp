#include <vector>
#include <sistema.h>
#include "gtest/gtest.h"
#include "factories.h"
#include "auxiliares.h"

TEST(test_sistema_volar_y_sensar, todo_libre) {
    Posicion posG = {0, 0};
    Posicion posC = {2, 2};
    std::vector<Drone> ds;
    ds.push_back(Drone(1, algunos_productos()));
    ds.push_back(Drone(2, algunos_productos()));
    ds.push_back(Drone(3, algunos_productos()));
    Campo c1 = Campo(posG, posC);
    Sistema s1(c1, ds);
    s1._estado.parcelas[0][1] = NoSensado;
    s1._estado.parcelas[1][0] = NoSensado;
    std::vector<std::vector<EstadoCultivo> > pre(s1._estado.parcelas);
    s1.volarYSensar(ds.back());

    EXPECT_EQ(c1, s1.campo());
    for (int i = 0; i < s1.campo().dimensiones().ancho; i++) {
        for (int j = 0; j < s1.campo().dimensiones().largo; j++) {
            if ((i == 0 && j == 1) || (i == 1 && j == 0)) continue;
            if (s1.campo().contenido(Posicion{i, j}) != Cultivo) continue;
            EXPECT_EQ(pre[i][j], s1.estadoDelCultivo({i, j}));
        }
    }
    Drone pre_cambiado = ds.back();
    Drone cambiado;
    bool encontrado = false;
    ds.pop_back();
    std::vector<Drone> sin_cambiado;
    const std::vector<Drone> &enjambre = s1.enjambreDrones();
    for (int i = 0; i < enjambre.size(); ++i) {
        if (enjambre[i].id() != pre_cambiado.id()) {
            sin_cambiado.push_back(enjambre[i]);
        } else {
            cambiado = enjambre[i];
            encontrado = true;
        }
    }
    EXPECT_TRUE(encontrado);
    Posicion opt1 = {0, 1};
    Posicion opt2 = {1, 0};
    EXPECT_TRUE(
            cambiado.posicionActual() == opt1 ||
            cambiado.posicionActual() == opt2);
    EXPECT_TRUE(test::mismos(sin_cambiado, ds));
    EXPECT_FALSE(s1.estadoDelCultivo(cambiado.posicionActual()) == NoSensado);
}

