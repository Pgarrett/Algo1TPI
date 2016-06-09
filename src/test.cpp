#include <iostream>
#include <sstream>
#include <fstream>

#include "tipos.h"
#include "campo.h"
#include "drone.h"
#include "sistema.h"

#include "auxiliares.cpp"

using namespace std;

inline std::ostream & operator<<(std::ostream & os, const InfoVueloCruzado & i)
{
  os << i.posicion << " " << i.cantidadCruces;
  return os;
}

Campo un_campo() {
    return Campo({5, 15}, {10, 20});
}

std::vector<Producto> algunos_productos() {
    return {HerbicidaLargoAlcance, PlaguicidaBajoConsumo, Herbicida};
}

std::vector<Producto> otros_productos() {
    return {Fertilizante, PlaguicidaBajoConsumo, Herbicida};
}

std::vector<Producto> algunos_productos_pero_en_otro_orden() {
    return {HerbicidaLargoAlcance, Herbicida, PlaguicidaBajoConsumo};
}

Drone un_drone() {
    return Drone(1, algunos_productos());
}

inline void campo_test()
{
  Posicion p(1,2);
  Posicion g(3,1);

  Campo c(p, g);
  g.x = 4;
  Campo c2(p, g);

  cout << (c == c2);
  cout << endl;
  istringstream iss("{ C [3,3] [[Cultivo,Cultivo,Granero],[Cultivo,Casa,Cultivo],[Cultivo,Cultivo,Cultivo]]}");
  c.cargar(iss);
  c.guardar(cout);
  cout << endl;
  cout << endl;

  Campo c3;
  std::ifstream in("test_campo.txt");
  c3.cargar(in);
  in.close();
  c3.guardar(cout);
  cout << endl;
  cout << endl;

  Campo c4({0,0}, {0, 1}, {3, 3});
  c4.guardar(cout);
  cout << endl;
  cout << endl;
}

inline void drone_test()
{
  vector<Producto> ps;
  ps.push_back(Plaguicida);
  ps.push_back(Herbicida);
  Drone d(1, ps);
  d.guardar(cout);
  Drone d2 = d;
  cout << "Expected true: " << (d==d) << (d==d2);
  d2.sacarProducto(Plaguicida);
  cout << "Expected false: " << (d==d2);

  Posicion p(1,2);
  d.moverA(p);
  p.y = 1;
  d.moverA(p);
  p.x = 1;
  p.y = 2;
  d.moverA(p);

  p.x = 1;
  p.y = 2;
  d2.moverA(p);
  p.y = 1;
  d2.moverA(p);
  p.x = 1;
  p.y = 2;
  d2.moverA(p);

  Secuencia<Drone> ds;
  ds.push_back(d);
  ds.push_back(d2);

  cout << endl;
  d.guardar(cout);
  cout << endl;
  cout << d.vueloEscalerado() << endl << endl;
  d.guardar(cout);
  cout << endl;
  d2.guardar(cout);
  cout << endl;
  mostrarSecuencia(cout, Drone::vuelosCruzados(ds));
  cout << endl;
  //istringstream iss("{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [PlaguicidaBajoConsumo,Herbicida,Fertilizante] true [2,0]}");
  istringstream iss("{ D 1 100 [] [HerbicidaLargoAlcance,PlaguicidaBajoConsumo,Herbicida] false [1,3]}");
  d.cargar(iss);
  d.guardar(cout);
  cout << endl;
  cout << endl;
}

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

inline void sistema_test()
{
  Posicion p(1,2);
  Posicion g(3,1);

  Campo c(p, g);
  g.x = 4;
  Campo c2(p, g);

  istringstream iss("{ C [3,3] [[Cultivo,Cultivo,Granero],[Cultivo,Casa,Cultivo],[Cultivo,Cultivo,Cultivo]]}");
  c.cargar(iss);

  vector<Producto> ps;
  ps.push_back(Plaguicida);
  ps.push_back(Herbicida);
  ps.push_back(Herbicida);
  ps.push_back(Herbicida);
  Drone d(1, ps);
  Drone d2(2, ps);

  d.moverA(p);
  p.y = 1;
  d.moverA(p);
  p.x = 1;
  p.y = 2;
  d.moverA(p);

  p.x = 1;
  p.y = 2;
  d2.moverA(p);
  p.y = 1;
  d2.moverA(p);
  p.x = 1;
  p.y = 2;
  d2.moverA(p);

  Secuencia<Drone> ds;
  ds.push_back(d);
  ds.push_back(d2);

  Sistema s(c, ds);
  s.seVinoLaMaleza(d2.vueloRealizado());
  
  Posicion posG = {0, 0};

  Drone elDrone = un_drone();
  elDrone.cambiarPosicionActual(posG);

  Campo el_campo(posG, {2, 2});
  Sistema s1(el_campo, {elDrone});

  s1.despegar(elDrone);
  
  Drone dd = buscarDroneEnSistema(elDrone, s1);
  
  cout << s1 << dd;

  //s.guardar(cout);
  cout << endl;
  cout << endl;
  istringstream isS("{ S { C [3,3] [[Cultivo,Cultivo,Granero],[Cultivo,Casa,Cultivo],[Cultivo,Cultivo,Cultivo]]} [{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [PlaguicidaBajoConsumo,Herbicida,Fertilizante] true [2,0]}, { D 15 46 [[0,1],[1,1],[2,1],[2,2]] [HerbicidaLargoAlcance,Fertilizante,Plaguicida] true [2,2]}] [[NoSensado,EnCrecimiento,NoSensado],[ConMaleza,NoSensado,ConPlaga],[EnCrecimiento,ListoParaCosechar,ConPlaga]]}");
  s.cargar(isS);
  //s.guardar(cout);
  cout << endl;
  cout << endl;
}
