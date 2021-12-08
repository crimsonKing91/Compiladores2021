#include<iostream>
#include<fstream>
#include<string>
#include<vector>	
using namespace std;

class Gramatica{
private:
	vector<char> terminales;
	vector<char> noTerminales;
	vector<string> producciones;

	void agregarTerminal(char terminal){
		if(buscarTerminal(terminal) >= 0)
			return;
		terminales.push_back(terminal);
	}
	void agregarNoTerminal(char noTerminal){
		if(buscarNoTerminal(noTerminal) >= 0)
			return;
		noTerminales.push_back(noTerminal);
	}
	bool esSimboloTerminal(char simbolo){
		if(simbolo >= 'a' && simbolo <= 'z')
			return true;
		return false;	
	}
	bool esSimboloNoTerminal(char simbolo){
		if(simbolo >= 'A' && simbolo <= 'Z' && simbolo != 'E')
			return true;
		return false;
	}
public:
	void agregarProduccion(string produccion){
		for(char simbolo: produccion){
			if(esSimboloTerminal(simbolo))
				agregarTerminal(simbolo);
			else if(esSimboloNoTerminal(simbolo))
				agregarNoTerminal(simbolo);
		}
		producciones.push_back(produccion);
	}
	int buscarTerminal(char simbolo){
		int i = 0;
		for(i = 0; i < terminales.size(); i++)
			if(terminales[i] == simbolo)
				return i;
		return -1;
	}
	int buscarNoTerminal(char simbolo){
		int i = 0;
		for(i = 0; i < noTerminales.size(); i++)
			if(noTerminales[i] == simbolo)
				return i;
		return -1;
	}
	int cantidadTerminales(){
		return terminales.size();
	}
	int cantidadNoTerminales(){
		return noTerminales.size();
	}
	vector<char> obtenerTerminales(){
		return terminales;
	}
	vector<char> obtenerNoTerminales(){
		return noTerminales;
	}
	vector<string> obtenerProducciones(){
		return producciones;
	}
};

/*Formato de gramática:
	La primer producción es el simbolo inicial.
	Las producciones son del tipo: S->aAbB, sin espacios (MUY IMPORTANTE)
*/
class LectorGramatica{
private:
	Gramatica gramatica;
public:
	Gramatica cargarGramatica(string ruta){
		ifstream archivoEntrada;
		string lineaBuffer = "", produccionBuffer = "";

		archivoEntrada.open(ruta);
	    while (archivoEntrada >> lineaBuffer) {
    		produccionBuffer = "";
	    	for(char simbolo: lineaBuffer){
	    		if((simbolo >= 'A' && simbolo <= 'Z') || (simbolo >= 'a' && simbolo <= 'z'))
	    			produccionBuffer += simbolo;
	    	}
	    	gramatica.agregarProduccion(produccionBuffer);
	    }
	    archivoEntrada.close();
	    return gramatica;
	}
};

class TablaLL1{
private:
	vector<vector<int>> tabla;
public:
	TablaLL1(int cantidadNoTerminales, int cantidadTerminales){
		tabla = vector<vector<int>>(cantidadNoTerminales, vector<int>(cantidadTerminales, -1));
	}
	TablaLL1(){
		tabla = vector<vector<int>>(0);
	}
	void agregarProduccion(char terminal, char noTerminal, int produccion, Gramatica gramatica){
		if(terminal == '$' && tabla[gramatica.buscarNoTerminal(noTerminal)][tabla[0].size()-1] < 0){
			tabla[gramatica.buscarNoTerminal(noTerminal)][tabla[0].size()-1] = produccion;
		}
		else if(tabla[gramatica.buscarNoTerminal(noTerminal)][gramatica.buscarTerminal(terminal)] < 0){
			tabla[gramatica.buscarNoTerminal(noTerminal)][gramatica.buscarTerminal(terminal)] = produccion;
		}
	}
	int obtenerProduccion(char terminal, char noTerminal, Gramatica gramatica){
		if(terminal == '$') return tabla[gramatica.buscarNoTerminal(noTerminal)][tabla[0].size()-1];
		return tabla[gramatica.buscarNoTerminal(noTerminal)][gramatica.buscarTerminal(terminal)];
	}
};

class EscritorTablaLL1{
public:
	void escribirTablaLL1(TablaLL1 tablaLL1, Gramatica gramatica, string ruta){
		ofstream archivoSalida;
		vector<char> terminales, noTerminales;
		terminales = gramatica.obtenerTerminales();
		terminales.push_back('$');
		noTerminales = gramatica.obtenerNoTerminales();
		string lineaBuffer = "";
		int produccion = 0;

		archivoSalida.open(ruta);
		for(char terminal: terminales)
			archivoSalida << "\t" << terminal;
		archivoSalida << endl;
		for(char noTerminal: noTerminales){
			archivoSalida << noTerminal;
			for(char terminal: terminales){
				produccion = tablaLL1.obtenerProduccion(terminal, noTerminal, gramatica);
				if(produccion >= 0)
					archivoSalida << "\t" << produccion;
				else
					archivoSalida << "\t ";
			}
			archivoSalida << endl;
		}
	    archivoSalida.close();
	}
};

class CalculadorPrimero{
private:
	int buscarSimboloEnConjunto(vector<char> conjunto, char S){
		int i;
		for(i = 0; i < conjunto.size(); i++)
			if(conjunto[i] == S){
				return i;
			}
		return -1;
	}
	bool contieneEpsilon(vector<char> &conjunto){
		return buscarSimboloEnConjunto(conjunto, 'E') >= 0;
	}
	void quitarEpsilon(vector<char> &conjunto){
		int pos;
		pos = buscarSimboloEnConjunto(conjunto, 'E');
		if(pos >= 0){
			conjunto.erase(conjunto.begin() + pos);
		}
	}
	void combinarConjuntos(vector<char> &conjuntoDestino, vector<char> conjuntoAuxiliar){
		for(char simbolo: conjuntoAuxiliar){
			if(buscarSimboloEnConjunto(conjuntoDestino, simbolo) < 0){
				conjuntoDestino.push_back(simbolo);
			}
		}
	}
public:
	void calcularPrimero(char X, Gramatica gramatica, vector<char> &conjuntoPrimero){
		if(X == 'E' || gramatica.buscarTerminal(X) >= 0){
			conjuntoPrimero.push_back(X);
		}
		else if(gramatica.buscarNoTerminal(X) >= 0){
			for(string produccion: gramatica.obtenerProducciones()){
				if(produccion[0] == X)
					calcularPrimero(produccion.substr(1), gramatica, conjuntoPrimero);
			}
		}
	}
	void calcularPrimero(string alfa, Gramatica gramatica, vector<char> &conjuntoPrimero){
		vector<char> conjuntoAuxiliar;
		int i;
		for(i = 0; i < alfa.size(); i++){
			conjuntoAuxiliar = vector<char>(0);
			calcularPrimero(alfa[i], gramatica, conjuntoAuxiliar);
			if(contieneEpsilon(conjuntoAuxiliar)){
				quitarEpsilon(conjuntoAuxiliar);
				combinarConjuntos(conjuntoPrimero, conjuntoAuxiliar);
			}
			else{
				combinarConjuntos(conjuntoPrimero, conjuntoAuxiliar);
				return;
			}
		}
		conjuntoPrimero.push_back('E');
	}
};

class CalculadorSiguiente{
private:
	int buscarSimboloEnCadena(string alfa, char X){
		int i;
		for(i = 0; i < alfa.size(); i++)
			if(alfa[i] == X)
				return i;
		return -1;
	}
	int buscarSimboloEnCadena(vector<char> alfa, char X){
		int i;
		for(i = 0; i < alfa.size(); i++)
			if(alfa[i] == X)
				return i;
		return -1;
	}
	void combinarConjuntos(vector<char> &conjuntoDestino, vector<char> conjuntoAuxiliar){
		for(char simbolo: conjuntoAuxiliar){
			if(buscarSimboloEnCadena(conjuntoDestino, simbolo) < 0){
				conjuntoDestino.push_back(simbolo);
			}
		}
	}
public:
	void calcularSiguiente(char X, Gramatica gramatica, vector<char> &conjuntoSiguiente){
		int indice = 0;
		vector<char> conjuntoAuxiliar;
		vector<string> producciones;
		CalculadorPrimero calculadorPrimero;
		producciones = gramatica.obtenerProducciones();

		if(producciones[0][0] == X){
			if(buscarSimboloEnCadena(conjuntoSiguiente, '$') < 0)
				conjuntoSiguiente.push_back('$');
		}
		for(string produccion: producciones){
			indice = buscarSimboloEnCadena(produccion.substr(1), X) + 1;
			if(indice == produccion.size() - 1 && produccion[0] != X){
				conjuntoAuxiliar = vector<char>(0);
				calcularSiguiente(produccion[0], gramatica, conjuntoAuxiliar);
				combinarConjuntos(conjuntoSiguiente, conjuntoAuxiliar);
			}
			else if(indice > 0 && indice < produccion.size() - 1){
				conjuntoAuxiliar = vector<char>(0);
				calculadorPrimero.calcularPrimero(produccion.substr(indice + 1), gramatica, conjuntoAuxiliar);
				if(buscarSimboloEnCadena(conjuntoAuxiliar, 'E') < 0){
					combinarConjuntos(conjuntoSiguiente, conjuntoAuxiliar);
				}
				else if(produccion[0] != X){
					conjuntoAuxiliar = vector<char>(0);
					calcularSiguiente(produccion[0], gramatica, conjuntoAuxiliar);
					combinarConjuntos(conjuntoSiguiente, conjuntoAuxiliar);
				}
			}
		}
	}
};

class CalculadorTablaLL1{
private:
	CalculadorPrimero calculadorPrimero;
	CalculadorSiguiente calculadorSiguiente;
	LectorGramatica lectorGramatica;
	EscritorTablaLL1 escritorTablaLL1;
	TablaLL1 tablaLL1;

	void guardarTablaLL1(string ruta, Gramatica gramatica){
		escritorTablaLL1.escribirTablaLL1(tablaLL1, gramatica, ruta);
	}
	int buscarSimboloEnConjunto(vector<char> conjunto, char S){
		int i;
		for(i = 0; i < conjunto.size(); i++)
			if(conjunto[i] == S){
				return i;
			}
		return -1;
	}
public:
	void crearTablaLL1(string rutaCarga, string rutaGuardado){
		vector<char> primeroAlfa;
		vector<char> siguienteA;
		vector<string> producciones;
		Gramatica gramatica;
		gramatica = lectorGramatica.cargarGramatica(rutaCarga);
		tablaLL1 = TablaLL1(gramatica.cantidadNoTerminales(), gramatica.cantidadTerminales() + 1);
		producciones = gramatica.obtenerProducciones();
		int i;
		for(i = 0; i < producciones.size(); i++){
			primeroAlfa = vector<char>(0);
			calculadorPrimero.calcularPrimero(producciones[i].substr(1), gramatica, primeroAlfa);
			if(buscarSimboloEnConjunto(primeroAlfa, 'E') < 0){
				for(char simboloPrim: primeroAlfa){
					tablaLL1.agregarProduccion(simboloPrim, producciones[i][0], i, gramatica);
				}
			}
			else{
				siguienteA = vector<char>(0);
				calculadorSiguiente.calcularSiguiente(producciones[i][0], gramatica, siguienteA);
				for(char simboloSig: siguienteA){
					tablaLL1.agregarProduccion(simboloSig, producciones[i][0], i, gramatica);
				}
			}
		}
		guardarTablaLL1(rutaGuardado, gramatica);
	}
};
