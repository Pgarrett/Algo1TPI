#include "sistema.h"

Sistema::Sistema()
{
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
}

const Campo & Sistema::campo() const
{
  return _campo;
}

EstadoCultivo Sistema::estadoDelCultivo(const Posicion & p) const
{
  return _estado.parcelas[p.x][p.y];
}

const Secuencia<Drone>& Sistema::enjambreDrones() const
{
  return _enjambre;
}

void Sistema::crecer()
{
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
}

void Sistema::seExpandePlaga()
{
}

void Sistema::despegar(const Drone & d)
{
}

bool Sistema::listoParaCosechar() const
{
	return false;
}

void Sistema::aterrizarYCargarBaterias(Carga b)
{
}

void Sistema::fertilizarPorFilas()
{
}

void Sistema::volarYSensar(const Drone & d)
{
}

void Sistema::mostrar(std::ostream & os) const
{
  os << "{ S ";
  _campo.mostrar(os);
  for(int i = 0; i < _enjambre.size(); i++)
  {
    _enjambre[i].mostrar(os);
  }
  os << " }";
}

//{ S { C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]} [{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]}, { D 15 46 [[0,1],[1,1][2,1][2,2]] [HerbicidaLargoAlcance, Fertilizante, Herbicida, Plaguicida]}] [[NoSensado,EnCrecimiento,NoSensado], [ConMaleza,NoSensado,ConPlaga], [EnCrecimiento,ListoParaCosechar, ConPlaga]] }
void Sistema::guardar(std::ostream & os) const
{
  os << "{ S ";
  _campo.guardar(os);
  os << " [";
  for(int i = 0; i < _enjambre.size(); i++)
  {
    _enjambre[i].mostrar(os);
    if (i < _enjambre.size() - 1)
      os << ",";
  }
  os << "]";
  Dimension d = _campo.dimensiones();
  for(int i=0; i<d.ancho; i++)
  {
    os << "[";
    Posicion p1 = Posicion();
    p1.x = i;
    p1.y = 0;
    os << _campo.contenido(p1);
    for(int j=1; j<d.largo; j++)
    {
      Posicion p2 = Posicion();
      p2.x = i;
      p2.y = 0;
      os << "," << _campo.contenido(p2);
    }

    if(i<d.ancho-1)
      os << "], ";
  }
  os << "]]}";
}

// { S 
//  { C [3,3] [[Cultivo,Cultivo,Granero], [Cultivo,Casa,Cultivo], [Cultivo, Cultivo,Cultivo]]} 
//  [{ D 12 83 [[1,2],[1,1],[1,0],[2,0]] [Plaguicida, PlaguicidaBajoConsumo, Herbicida, Fertilizante]}, 
//   { D 15 46 [[0,1],[1,1][2,1][2,2]] [HerbicidaLargoAlcance, Fertilizante, Herbicida, Plaguicida]}
//  ] 
//  [[NoSensado,EnCrecimiento,NoSensado], [ConMaleza,NoSensado,ConPlaga], [EnCrecimiento,ListoParaCosechar, ConPlaga]] }
void Sistema::cargar(std::istream & is)
{
  // TODO clear de campo?
  _enjambre.clear();
  _estado.clear(); // TODO check
  char b;
  is >> b >> b;
  _campo.cargar(is);
  // string ingresaDrones;
  // getline(is, ingresaDrones, '[');
  // char ultimoDrone = *ingresaDrones.rbegin();
  // while(ultimoDrone != ']')
  // {
  //   Drone droneCargado;
  //   droneCargado.cargar(is);
  //   _enjambre.push_back(droneCargado);
  // }
  string todosLosDrones;
  getline(is, todosLosDrones, ']');
  vector<string> drones = split(todosLosDrones, ',');
  for(int i = 0; i < drones.size(); i++)
  {
    stringstream droneStream(drones[i]);
    Drone droneCargado;
    droneCargado.cargar(droneStream);
    _enjambre.push_back(droneCargado);
  }

  Dimension dimensionCampo = _campo.dimensiones();
  for(int i=0; i<dimensionCampo.ancho; i++)
  {
    is >> b;
    string estadoC;
    Posicion p1;
    p1.x = i;
    p1.y = 0;
    if(_campo.contenido(p1) == Cultivo)
    {
      //_estado.parcelas[i][0]
    }
    for(int j=1; j<dimensionCampo.largo; j++)
    {
      string parcela;
      getline(is, parcela, ',');
      _grilla.parcelas[i][0] = tipoDeParcela(parcela);
    }
    is >> b;
  }
}

bool Sistema::operator==(const Sistema & otroSistema) const
{
	return false;
}

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	return os;
}iostream
