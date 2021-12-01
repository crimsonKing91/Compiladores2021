#include<bits/stdc++.h>
#include"PseudoAF.cpp"
#include"AF.cpp"
using namespace std;

class Thompson{
private:
	vector<tuple<int, int, char>> transiciones;
	int ult_id = 0;
	string reescribir_expresion(string exp){
		int i, j = 0, nivel = 0, tam = exp.size();
		string exp_aumentada = "", aux = "";
		for(i = 0; i < tam; i++){
			if(nivel == 0){
				if(exp[i] == '(')
					nivel++;
				else{
					if(!(exp[i] == '*' && exp[i - 1] == '*'))
						exp_aumentada += exp[i];
					if((i + 1) < tam && exp[i + 1] == '+'){
						if(exp[i] != '*') exp_aumentada += exp[i];
						exp[i + 1] = '*';
					}
				}
			}
			else{
				if(exp[i] == '(')
					nivel++;
				else if(exp[i] == ')' && nivel > 0)
					nivel--;

				if(exp[i] == ')' && nivel == 0){
					aux = reescribir_expresion(aux);
					exp_aumentada += '(' + aux + ')';
					if((i + 1) < tam && exp[i + 1] == '+'){
						exp_aumentada += '(' + aux + ')';
						exp[i + 1] = '*';
					}
					aux = "";
				}
				if(nivel > 0)
					aux += exp[i];
			}
		}
		return exp_aumentada;
	}
	int nuevo_estado(){
		ult_id++;
		return ult_id;
	}
	void crear_transicion(int inicial, int final, char simbolo){
		transiciones.push_back(make_tuple(inicial, final, simbolo));
	}
	PseudoAF cerradura_estrella(PseudoAF pseudo_af){
		int estado1, estado2;
		estado1 = nuevo_estado();
		estado2 = nuevo_estado();
		crear_transicion(estado1, pseudo_af.ini(), 'E');
		crear_transicion(pseudo_af.fin(), estado2, 'E');
		crear_transicion(pseudo_af.fin(), pseudo_af.ini(), 'E');
		crear_transicion(estado1, estado2, 'E');
		return PseudoAF(estado1, estado2);
	}
	PseudoAF concatenar(PseudoAF af_izq, PseudoAF af_der){
		int fin_af_izq = af_izq.fin();
		int ini_af_der = af_der.ini();
		int i, aux;
		for(i = 0; i < transiciones.size(); i++){
			if(get<0>(transiciones[i]) == ini_af_der){
				transiciones[i] = make_tuple(fin_af_izq, get<1>(transiciones[i]), get<2>(transiciones[i]));
			}
		}
		return PseudoAF(af_izq.ini(), af_der.fin());
	}
	PseudoAF unir(PseudoAF af_sup, PseudoAF af_inf){
		int estado1, estado2;
		estado1 = nuevo_estado();
		estado2 = nuevo_estado();
		crear_transicion(estado1, af_sup.ini(), 'E');
		crear_transicion(estado1, af_inf.ini(), 'E');
		crear_transicion(af_sup.fin(), estado2, 'E');
		crear_transicion(af_inf.fin(), estado2, 'E');
		return PseudoAF(estado1, estado2);
	}
	vector<string> formatearAFN(PseudoAF pre_afn){
		vector<string> afn;
		string cadena;
		afn.push_back("inicial:" + to_string(pre_afn.ini()));
		afn.push_back("finales:" + to_string(pre_afn.fin()));
		for(tuple<int, int, char> tupla: transiciones){
			cadena = to_string(get<0>(tupla)) + "->" + to_string(get<1>(tupla)) + "," + get<2>(tupla);
			afn.push_back(cadena);
		}
		return afn;
	}
	PseudoAF obtener_infoAFN(string exp){
		int i, nivel = 0, estado1, estado2;
		vector<string> afn;
		string aux = "";
		vector<PseudoAF> automatas(exp.size());
		for(i = 0; i < exp.size(); i++){
			if(exp[i] != '*' && exp[i] != '|' && exp[i] != '(' && exp[i] != ')' && nivel == 0){
				estado1 = nuevo_estado();
				estado2 = nuevo_estado();
				automatas[i] = PseudoAF(estado1, estado2);
				crear_transicion(automatas[i].ini(), automatas[i].fin(), exp[i]);
			}
			else if(exp[i] == '('){
				if(nivel == 0){
					exp.erase(exp.begin() + i);
					automatas.erase(automatas.begin() + i);
					i--;
				}
				else{
					aux += exp[i];
					exp.erase(exp.begin() + i);
					automatas.erase(automatas.begin() + i);
					i--;
				}
				nivel++;
			}
			else if(exp[i] == ')'){
				nivel--;
				if(nivel == 0){
					automatas[i] = obtener_infoAFN(aux);
					aux = "";
				}
				else{
					aux += exp[i];
					exp.erase(exp.begin() + i);
					automatas.erase(automatas.begin() + i);
					i--;
				}
			}
			else if(nivel > 0){
				aux += exp[i];
				exp.erase(exp.begin() + i);
				automatas.erase(automatas.begin() + i);
				i--;
			}
		}
		for(i = 0; i < exp.size(); i++){
			if(exp[i] == '*'){
				automatas[i] = cerradura_estrella(automatas[i - 1]);
				exp.erase(exp.begin() + i - 1);
				automatas.erase(automatas.begin() + i - 1);
				i--;
			}
		}
		for(i = 1; i < exp.size(); i++){
			if(exp[i] != '|' && exp[i - 1] != '|'){
				automatas[i] = concatenar(automatas[i - 1], automatas[i]);
				exp.erase(exp.begin() + i - 1);
				automatas.erase(automatas.begin() + i - 1);
				i--;
			}
		}
		for(i = 1; i < exp.size() - 1; i++){
			if(exp[i] == '|'){
				automatas[i] = unir(automatas[i - 1], automatas[i + 1]);
				exp.erase(exp.begin() + i + 1);
				automatas.erase(automatas.begin() + i + 1);
				exp.erase(exp.begin() + i - 1);
				automatas.erase(automatas.begin() + i - 1);
				i--;
			}
		}
		return automatas[0];
	}
	/*AFN crearAFN(vector<string> info_afn){
		AFN afn;
		ofstream archivo_aux;
	    archivo_aux.open("aux.af");
		for(string linea: info_afn)
			archivo_aux << linea << endl;
	    archivo_aux.close();
	    afn.cargar_desde("aux.af");
	    remove("aux.af");
	    return afn;
	}*/
	void crearAFN(vector<string> info_afn){
		AFN afn;
		ofstream archivo_aux;
	    archivo_aux.open("aux.af");
		for(string linea: info_afn)
			archivo_aux << linea << endl;
	    archivo_aux.close();
	    afn.cargar_desde("aux.af");
	    remove("aux.af");
	    afn.guardar_en("a.log");
	}
public:
	void convertir(string exp_reg){
		PseudoAF info_afn;
		string exp_reescrita;
		exp_reescrita = reescribir_expresion(exp_reg);
		info_afn = obtener_infoAFN(exp_reescrita);
		crearAFN(formatearAFN(info_afn));
	}
	/*AFN convertir(string exp_reg){
		PseudoAF info_afn;
		string exp_reescrita;
		exp_reescrita = reescribir_expresion(exp_reg);
		info_afn = obtener_infoAFN(exp_reescrita);
		return crearAFN(formatearAFN(info_afn));
	}*/
};