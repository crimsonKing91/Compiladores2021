#include<vector>
using namespace std;

class Estado{
private:
	int buscar_transicion(int estado_objetivo, char simbolo){
		int i;
		for (i = 0; i < transiciones.size(); i++){
			if (transiciones[i].estado_objetivo == estado_objetivo && transiciones[i].simbolo == simbolo)
				return i;
		}
		return -1;
	}
public:
	vector<Transicion> transiciones;

	Estado(){
		transiciones = vector<Transicion>();
	}
	Estado(vector<Transicion> transiciones){
		this->transiciones = transiciones;
	}
	void agregar_transicion(Transicion transicion){
		transiciones.push_back(transicion);
	}
	void eliminar_transicion(int estado_objetivo, char simbolo){
		int transicion;
		transicion = buscar_transicion(estado_objetivo, simbolo);
		if (transicion != -1)
			transiciones.erase(transiciones.begin() + transicion);
	}
};