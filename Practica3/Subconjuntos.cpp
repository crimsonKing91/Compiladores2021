#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stack>
#include<set>
#include"AF.cpp"
using namespace std;

struct Transicion{
	int origen;
	int destino;
	char simbolo;
	Transicion(int origen, int destino, char simbolo){
		this->origen = origen;
		this->destino = destino;
		this->simbolo = simbolo;
	}
};

class Subconjuntos{
private:
	vector<Transicion> transiciones;
	vector<string> afd_formato_af;

	void cargar_transiciones(AFN afn){
		int estado_origen, estado_destino;
		char omitir_caracter, simbolo;
		string omitir_linea;
		ifstream archivo_auxiliar;

		afn.guardar_en("auxiliar.af");
		archivo_auxiliar.open("auxiliar.af");
		archivo_auxiliar >> omitir_linea >> omitir_linea;
	    while (archivo_auxiliar >> estado_origen) {
	    	archivo_auxiliar >> omitir_caracter >> omitir_caracter >> estado_destino >> omitir_caracter >> simbolo;
	    	transiciones.push_back(Transicion(estado_origen, estado_destino, simbolo));
	    }
	    archivo_auxiliar.close();
	    remove("auxiliar.af");
	}

	set<int> calcular_cerradura_epsilon(set<int> estados){
		if(estados.empty()) return estados;

		int estado_procesado = 0;
		stack<int> pila;
		for(int estado: estados){
			pila.push(estado);
		}
		while(!pila.empty()){
			estado_procesado = pila.top();
			pila.pop();
			for(Transicion transicion: transiciones){
				if(transicion.origen == estado_procesado && transicion.simbolo == 'E'){
					estados.insert(transicion.destino);
					pila.push(transicion.destino);
				}
			}
		}
		return estados;
	}

	set<int> mover(set<int> estados_origen, char simbolo){
		set<int> estados_destino;
		for(int estado_actual: estados_origen){
			for(Transicion transicion: transiciones){
				if(transicion.origen == estado_actual && transicion.simbolo == simbolo){
					estados_destino.insert(transicion.destino);
				}
			}
		}
		return estados_destino;
	}

	bool esta_en_subconjuntos(set<int> subconjunto, vector<set<int>> subconjuntos){
		for(set<int> subconjunto_actual: subconjuntos){
			if(subconjunto == subconjunto_actual) return true;
		}
		return false;
	}

	void crear_transicion_AFD(set<int> origen, set<int> destino, char simbolo){
		if(!origen.empty() && !destino.empty()){
			string etiqueta_origen = "", etiqueta_destino = "";
			for(int numero: origen){
				etiqueta_origen += to_string(numero);
			}
			for(int numero: destino){
				etiqueta_destino += to_string(numero);
			}
			afd_formato_af.push_back(etiqueta_origen + "->" + etiqueta_destino + "," + simbolo);
		}
	}

	vector<set<int>> calcular_transiciones_AFD(AFN afn){
		int iterador = 0, simbolo = 0;
		vector<set<int>> subconjuntos;
		set<int> subconjunto_procesado, subconjunto_siguiente;

		subconjunto_procesado.insert(afn.obtener_inicial());
		subconjuntos.push_back(calcular_cerradura_epsilon(subconjunto_procesado));
		for(iterador = 0; iterador < subconjuntos.size(); iterador++){
			subconjunto_procesado = subconjuntos[iterador];
			for(simbolo = (int)'a'; simbolo <= (int)'z'; simbolo++){
				subconjunto_siguiente = calcular_cerradura_epsilon(mover(subconjunto_procesado,(char)simbolo));
				if(!subconjunto_siguiente.empty() && !esta_en_subconjuntos(subconjunto_siguiente, subconjuntos)){
					subconjuntos.push_back(subconjunto_siguiente);
				}
				crear_transicion_AFD(subconjunto_procesado, subconjunto_siguiente, simbolo);
			}
		}
		return subconjuntos;
	}

	void agregar_inicial_y_final(AFN afn, vector<set<int>> subconjuntos){
		int inicial_afn = 0;
		vector<int> finales_afn;
		string estado_inicial = "inicial:", estados_finales = "finales:", etiqueta_auxiliar = "";
		set<string> etiquetas_finales;
		inicial_afn = afn.obtener_inicial();
		finales_afn = afn.obtener_finales();
		for(set<int> subconjunto: subconjuntos){
			if(subconjunto.find(inicial_afn) != subconjunto.end()){
				for(int numero: subconjunto){
					estado_inicial += to_string(numero);
				}
			}
		}
		for(int estado_final: finales_afn){
			for(set<int> subconjunto: subconjuntos){
				if(subconjunto.find(estado_final) != subconjunto.end()){
					for(int numero: subconjunto){
						etiqueta_auxiliar += to_string(numero);
					}
					etiquetas_finales.insert(etiqueta_auxiliar);
					etiqueta_auxiliar = "";
				}
			}
		}
		if(etiquetas_finales.size() == 1){
			estados_finales += *etiquetas_finales.begin();
		}
		else{
			set<string>::iterator it = etiquetas_finales.begin();
			estados_finales += *it;
			for(it = ++etiquetas_finales.begin(); it != etiquetas_finales.end(); it++){
				estados_finales += "," + *it;
			}
		}
		afd_formato_af.emplace(afd_formato_af.begin(), estados_finales);
		afd_formato_af.emplace(afd_formato_af.begin(), estado_inicial);
	}

	AFD generar_AFD(){
		AFD afd;
		int estado_origen, estado_destino;
		char simbolo;
		ofstream archivo_auxiliar;

		archivo_auxiliar.open("auxiliar.af");
		for(string linea: afd_formato_af){
			archivo_auxiliar << linea << endl;
		}
	    archivo_auxiliar.close();
	    afd.cargar_desde("auxiliar.af");
	    remove("auxiliar.af");
	    return afd;
	}

public:
	AFD convertir_AFN_a_AFD(AFN afn){
		vector<set<int>> subconjuntos;
		cargar_transiciones(afn);
		subconjuntos = calcular_transiciones_AFD(afn);
		agregar_inicial_y_final(afn, subconjuntos);
		return generar_AFD();
	}
};