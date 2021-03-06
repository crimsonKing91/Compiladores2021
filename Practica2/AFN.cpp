#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<fstream>
#include<cstdio>
#include<vector>
#include<queue>
#include<tuple>
#include"Transicion.cpp"
#include"Estado.cpp"
using namespace std;

class AFN{
private:
	vector<Estado> estados;
	int estado_inicial = 0;
	vector<int> estados_finales;

	void manejar_error(int codigo, string complemento){
	    time_t fecha = time(NULL);
		ofstream reporte_errores;

	    reporte_errores.open("error.log", ios_base::app);
	    reporte_errores << ctime(&fecha) << "Código de error: " << codigo << endl;
		switch(codigo){
			case 0: reporte_errores << "Error abriendo el archivo de carga";
				break;
			case 1: reporte_errores << "Argumento inválido";
				break;
			case 2: reporte_errores << "No se ha asignado ningún estado inicial";
				break;
		}
		reporte_errores << complemento << endl;
	    reporte_errores.close();
	}
	void manejar_error(int codigo){
		manejar_error(codigo, "");
	}
	void obtener_finales(string linea){
		int seleccion = 0;
		string aux = "";

		try{
			for(char letra: linea){
				switch(seleccion){
					case 0:
						if(letra == ':') seleccion++;
					break;
					case 1:
						if(letra == ','){
							estados_finales.push_back(stoi(aux));
							aux = "";
						}
						else
							aux += letra;
					break;
				}
			}
			estados_finales.push_back(stoi(aux));
		} catch(invalid_argument){
			manejar_error(1, " en el método \"obtener_finales\"");
		}
	}
	string generar_cadena(int estado, char simbolo, vector<bool> &estados_visitados){
		string cadena = "";
		estados_visitados[estado] = true;
		if (esFinal(estado)){
			return cadena + simbolo;
		}
		else{
			int i;
			int estado_siguiente;
			int transiciones = estados[estado].transiciones.size();
			for (i = 0; i < transiciones; i++){
				estado_siguiente = estados[estado].transiciones[i].estado_objetivo;
				if (estado != estado_siguiente && !estados_visitados[estado_siguiente])
					cadena = generar_cadena(estado_siguiente, estados[estado].transiciones[i].simbolo, estados_visitados);
				if (cadena != ""){
					return simbolo + cadena;
				}
			}
			return "";
		}
	}
	bool esFinal(int estado){
		for (int final: estados_finales){
			if (estado == final)
				return true;
		}
		return false;
	}
public:
	void cargar_desde(string nombre){
		//Carga el autómata desde una ruta especificada
		int i = 0, estado_aux, inicio, total_estados = 0;
		char skip, simbolo;
		string cadena_aux;
		vector<tuple<int, int, char>> entrada;
		ifstream archivo_entrada;
		Transicion transicion_aux;

		archivo_entrada.open(nombre);
	    if (!archivo_entrada) {
	    	manejar_error(0, " en el método \"cargar_desde\"");
	    }
	    else{
		    for (i = 0; i < 8; i++) archivo_entrada >> skip;
		    archivo_entrada >> estado_inicial;
			archivo_entrada >> cadena_aux;
			obtener_finales(cadena_aux);
		    while (archivo_entrada >> inicio) {
		    	archivo_entrada >> skip >> skip >> estado_aux >> skip >> simbolo;
		    	entrada.push_back(make_tuple(inicio, estado_aux, simbolo));
		    	total_estados = max(total_estados, inicio);
		    }
		}
	    archivo_entrada.close();
	    vector<vector<Transicion>> transiciones(total_estados + 1);
	    for (tuple<int, int, char> tupla: entrada){
	    	transicion_aux = Transicion(get<1>(tupla), get<2>(tupla));
	    	transiciones[get<0>(tupla)].push_back(transicion_aux);
	    }
	    estados = vector<Estado>(total_estados + 1);
	    for (i = 1; i <= total_estados; i++){
	    	estados[i] = Estado(transiciones[i]);
	    }
	}
	void guardar_en(string nombre){
		//Guarda el autómata en una ruta especificada, si el archivo especificado no existe entonces creará uno con ese nombre
		int i = 0, j = 0;
		vector<Transicion> transiciones;
		ofstream archivo_de_guardado;

	    archivo_de_guardado.open(nombre);
	    archivo_de_guardado << "inicial:" << estado_inicial << endl;
	    archivo_de_guardado << "finales:" << estados_finales[0];
	    for (i = 1; i < estados_finales.size(); i++)
	    	archivo_de_guardado << "," << estados_finales[i];
	    archivo_de_guardado << endl;
	    for (i = 0; i < estados.size(); i++){
	    	if(!estados[i].transiciones.empty()){
	    		transiciones = estados[i].transiciones;
	    		for (j = 0; j < transiciones.size(); j++){
	    			archivo_de_guardado << i << "->" << transiciones[j].estado_objetivo << "," << transiciones[j].simbolo << endl;
	    		}
	    	}
	    }
	    archivo_de_guardado.close();
	}
	void agregar_transicion(int inicio, int fin, char simbolo){
		//Agrega una transición al autómata desde el estado 'inicio' hacia el estado 'fin', con el simbolo especificado
		//Si el símbolo no está en el rango especificado, arrojará un error
		try{
			Transicion transicion = Transicion(fin, simbolo);
			estados[inicio].agregar_transicion(transicion);
		} catch (string error) {
		    manejar_error(1, ". " + error);
	    }
	}
	void eliminar_transicion(int inicio, int fin, char simbolo){
		//Elimina una transición que coincida con los parámetros
		estados[inicio].eliminar_transicion(fin, simbolo);
	}
	int obtener_inicial(){
		//Devuelve el estado inicial del autómata
		//Arroja un error si no existe un estado inicial
		if (estado_inicial < 1) manejar_error(2);
		return estado_inicial;
	}
	vector<int> obtener_finales(){
		//Devielve un vector con los estados finales del autómata
		return estados_finales;
	}
	void establecer_inicial(int estado){
		//Cambia el estado inicial actual a un estado normal y convierte en estado inicial el que se indica como parámetro
		estado_inicial = estado;
	}
	void establecer_final(int estado){
		//Agraga un estado final a la lista de estados finales
		if (!esFinal(estado)) estados_finales.push_back(estado);
	}
	bool esAFN(){
		//Devuelve true si el autómata que ha sido cargado es un AFN, false en caso contrario
		queue<int> cola;
		cola.push(estado_inicial);
		int estado_actual, estado_siguiente;
		int i;
		vector<int> simbolos;
		vector<bool> estados_visitados(estados.size() + 1, false);

		/*Para verificar si es un AFN se usa una búsqueda de amplitud para encontrar algúna transicion epsilon
		o un estado con dos transiciones para el mismo simbolo. Si las hay devuelve verdadero, falso en caso contrario*/
		while (!cola.empty()){
			estado_actual = cola.front();//Se usa para la búsqueda de amplitud
			estados_visitados[estado_actual] = true;
			cola.pop();
			bool transicionEpsilon = false;
			simbolos = vector<int>(26, 0);
			vector<Transicion> transiciones = estados[estado_actual].transiciones;
			for (i = 0; i < transiciones.size(); i++){
				if ((transiciones[i].simbolo - 'a' >= simbolos.size() || transiciones[i].simbolo - 'a' < 0) && transiciones[i].simbolo != 'E')
					return false; /*Devuelve false si encuentra un simbolo que no esté entre a y z,
					ya que para este caso, esos son caracteres inválidos*/
				if (transiciones[i].simbolo == 'E')
					transicionEpsilon = true;
				if (simbolos[transiciones[i].simbolo - 'a'] == 0 && transiciones[i].simbolo != 'E')
					/*transiciones[i].simbolo - 'a' devuelve un número correpondiente a cada letra; 0 para la 'a', 1 para la 'b', etc.*/
					simbolos[transiciones[i].simbolo - 'a']++;
				else if (simbolos[transiciones[i].simbolo - 'a'] == 1)
					return true;

				estado_siguiente = transiciones[i].estado_objetivo;
				if (estado_siguiente > 0 && !esFinal(estado_siguiente) && !estados_visitados[estado_siguiente])
					cola.push(estado_siguiente);
			}
			return transicionEpsilon;
		}
	}
	bool esAFD(){
		//Devuelve true si el autómata que ha sido cargado es un AFD, false en caso contrario
		queue<int> cola;
		cola.push(estado_inicial);
		int estado_actual, estado_siguiente;
		int i;
		vector<int> simbolos;
		vector<bool> estados_visitados(estados.size() + 1, false);

		/*Para verificar si es un AFD se usa una búsqueda de amplitud para encontrar algúna transicion epsilon
		o un estado con dos transiciones para el mismo simbolo. Si las hay devuelve falso, verdadero en caso contrario*/
		while (!cola.empty()){
			estado_actual = cola.front();//Se usa para la búsqueda de amplitud
			estados_visitados[estado_actual] = true;
			cola.pop();
			simbolos = vector<int>(26, 0);
			vector<Transicion> transiciones = estados[estado_actual].transiciones;
			for (i = 0; i < transiciones.size(); i++){
				if (transiciones[i].simbolo == 'E')
					return false; /*Devuelve false si encuentra un simbolo que no esté entre a y z,
					ya que para este caso, esos son caracteres inválidos*/
				if (transiciones[i].simbolo - 'a' >= simbolos.size() || transiciones[i].simbolo - 'a' < 0)
					return false; //Devuelve false si encuentra un simbolo que no esté entre a y z
				if (simbolos[transiciones[i].simbolo - 'a'] == 0)
				/*transiciones[i].simbolo - 'a' devuelve un número correpondiente a cada letra; 0 para la 'a', 1 para la 'b', etc.*/
					simbolos[transiciones[i].simbolo - 'a']++;
				else if (simbolos[transiciones[i].simbolo - 'a'] == 1)
					return false;
				estado_siguiente = transiciones[i].estado_objetivo;
				if (estado_siguiente > 0 && !esFinal(estado_siguiente) && !estados_visitados[estado_siguiente])
					cola.push(estado_siguiente);
			}
		}
		return true;
	}
	bool acepta(string cadena){
		//Devuelve true si el autómata reconoce esa cadena, false en caso contrario
		int i, j;
		int estado_actual;
		queue<int> cola, auxiliar;
		Transicion transicion_actual;
		cola.push(estado_inicial);
		//Aquí se realiza una bfs para buscar si la cadena es aceptada
		for (i = 0; i < cadena.size(); i++){
			while (!cola.empty()){
				estado_actual = cola.front();
				cola.pop();
				if (estado_actual > 0 && !esFinal(estado_actual)){
					for (j = 0; j < estados[estado_actual].transiciones.size(); j++){
						transicion_actual = estados[estado_actual].transiciones[j];
						if (transicion_actual.simbolo == cadena[i])
							auxiliar.push(transicion_actual.estado_objetivo);
					}
				}
			}
			while (!auxiliar.empty()){
				cola.push(auxiliar.front());
				auxiliar.pop();
			}
		}
		/*Al finalizar el análisis de la cadena caracter por caracter se revisa si
		alguno de los estados a los que se llegó es un estado final*/
		while (!cola.empty()){
			if (esFinal(cola.front())) //Si alguno es un estado final entonces la cadena es aceptada
				return true;
			cola.pop();
		}
		return false; //Si no se llegó a ningún estado final, entonces la cadena no es aceptada
	}
	string generar_cadena(){
		//Genera una cadena aleatoria que es aceptada por el autómata
		int i = 0;
		string cadena = "";
		vector<bool> estados_visitados(estados.size() + 1, false);
		//Aquí se realiza un dfs con llamadas recursivas para generar una caena que sea aceptada por el autómata
		vector<Transicion> transiciones = estados[estado_inicial].transiciones;
		for(i = 0; i < transiciones.size(); i++){
			cadena = generar_cadena(transiciones[i].estado_objetivo, transiciones[i].simbolo, estados_visitados);
			if (cadena != "")
				break;
		}
		if (cadena == "")//En caso de que el autómata no acepte ninguna cadena, se arroja un error
			manejar_error(3);
		return cadena;
	}
};