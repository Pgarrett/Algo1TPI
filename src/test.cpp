#include <iostream>
#include <sstream>

#include "tipos.h"
#include "campo.h"
#include "drone.h"

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
  cout << endl;
  istringstream iss("{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]}");
  d.cargar(iss);
  d.guardar(cout);
  cout << endl;
  //TODO probar cargar un drone que tenga lista vacia de productosDisponibles
}