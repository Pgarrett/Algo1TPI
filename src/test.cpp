#include <iostream>
#include <sstream>

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

inline void campo_test()
{
  Posicion p;
  p.x = 1;
  p.y = 2;

  Posicion g;
  g.x = 3;
  g.y = 1;

  Campo c(p, g);
  g.x = 4;
  Campo c2(p, g);

  cout << (c == c2);
  istringstream iss("{ C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]}");
  c.cargar(iss);
  c.guardar(cout);
}

inline Campo dameCampo()
{
  Posicion p;
  p.x = 1;
  p.y = 2;

  Posicion g;
  g.x = 3;
  g.y = 1;

  Campo c(p, g);
  return c;
}

inline Secuencia<Drone> dameDrones()
{
  vector<Producto> ps;
  ps.push_back(Plaguicida);
  ps.push_back(Herbicida);
  Drone d(1, ps);
  d.guardar(cout);
  Drone d2 = d;
  d.sacarProducto(Plaguicida);
  Posicion p;
  p.x = 1;
  p.y = 2;
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
  return ds;
}

inline void drone_test()
{
  vector<Producto> ps;
  ps.push_back(Plaguicida);
  ps.push_back(Herbicida);
  Drone d(1, ps);
  d.guardar(cout);
  Drone d2 = d;
  d.sacarProducto(Plaguicida);
  d.guardar(cout);
  cout << (d==d) << (d==d2);
  d2.sacarProducto(Plaguicida);
  cout << (d==d) << (d==d2);

  Posicion p;
  p.x = 1;
  p.y = 2;
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

  d.guardar(cout);
  cout << d.vueloEscalerado() << endl << endl;
  d.guardar(cout);
  cout << endl;
  d2.guardar(cout);
  cout << endl;
  mostrarSecuencia(cout, Drone::vuelosCruzados(ds));
  istringstream iss("{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]}");
  d.cargar(iss);
}

inline void sistema_test()
{
  Sistema s = Sistema(dameCampo(), dameDrones());
  cout << endl;
  cout << endl;
  s.guardar(cout);
  cout << endl;
  cout << endl;
  istringstream iss("{ S { C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]} [{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]}, { D 15 46 [[0,1],[1,1],[2,1],[2,2]] [HerbicidaLargoAlcance, Fertilizante, Herbicida, Plaguicida]}] [[NoSensado,EnCrecimiento,NoSensado], [ConMaleza,NoSensado,ConPlaga], [EnCrecimiento,ListoParaCosechar, ConPlaga]] }");
  s.cargar(iss);
  cout << endl;
  cout << endl;
  s.guardar(cout);
  cout << endl;
  cout << endl;
}
