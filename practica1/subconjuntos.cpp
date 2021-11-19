/*Subconjuntos -- AFN -> AFD*/

#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#define EPSILON -1
#define ACEPTACION 21

vector<unordered_map<int,int>> transiciones{
{{0,0}},
{{2,-1},{21,-1}},
{{3,-1},{15,-1}},
{{4,-1},{11,-1}},
{{5,-1},{10,-1}},
{{6,97}},
{{7,-1},{9,-1}},
{{8,98}},
{{9,-1},{7,-1}},
{{10,-1},{5,-1}},
{{14,-1}},
{{12,97}},
{{13,98}},
{{14,-1}},
{{20,-1}},
{{16,97}},
{{17,-1},{19,-1}},
{{18,98}},
{{19,-1},{17,-1}},
{{20,-1}},
{{21,-1},{2,-1}}
};

/*
vector<unordered_map<int,int>> transiciones{
{{0,0}},
{{2,-1},{9,-1}},
{{3,-1},{6,-1}},
{{4,'a'}},
{{5,'b'}},
{{8,-1}},
{{7,'c'}},
{{8,-1}},
{{2,-1},{9,-1}},
{{10,'a'}}
};
*/
void cerradura_e(vector<int> &nodosMover,vector<int> &nodosCerradura,vector<char> &nodosAceptacion,char estado);
void meterEstado(vector<int> &nodosCerradura,vector<pair<char,vector<int>>> &tablaEstados,char &estadoN);
void mover(vector<int> &nodosCerradura,vector<int> &nodosMover,int c);
void print(vector<int> &nodosMover,char c,char estado, char estadoN);
void printP(vector<int> &nodosMover,char c,char estado, char estadoN);
void printR(vector<int> &nodosMover,char c,char estado, char estadoN);
char no_marcado(vector<int> nodosMover,vector<pair<char,vector<int>>> tablaEstados, char estadoN);
void aceptacion(vector<char> &nodosAceptacion);
void printTablaEstados(vector<pair<char,vector<int>>> &tablaEstados);


int main(){
	vector<int> nodosMover;
	vector<int> nodosCerradura;
	vector<char> nodosAceptacion;
	vector<pair<char,vector<int>>> tablaEstados;
	char estadoN = 'A';
	string alfabeto = "ab";

	//Cerradura de 0
	cerradura_e(nodosMover,nodosCerradura,nodosAceptacion,'A');
	//Se copia la cerradura de 0 a la tabla de estados
	meterEstado(nodosCerradura,tablaEstados,estadoN);

	//Se recorre el alfabeto y se aplica la operacion 'mover'
	for(int i=0;i<alfabeto.size();i++){
		nodosMover.clear();
		mover(nodosCerradura,nodosMover,alfabeto[i]);
		if(nodosMover[0] != -1)
			print(nodosMover,alfabeto[i],'A',estadoN);
		else printP(nodosMover,alfabeto[i],'A',estadoN);
		meterEstado(nodosMover,tablaEstados,estadoN);
	}

	for(int k=1;k<tablaEstados.size();k++){
		nodosMover.clear();
		//Si es el estado pozo se omite
		if(tablaEstados[k].second[0] == -1) continue;
		//Se copia el siguiente estado no procesado
		for(int j=0;j<tablaEstados[k].second.size();j++)
			nodosMover.push_back(tablaEstados[k].second[j]);
		//Se le aplica la cerradura
		cerradura_e(nodosMover,nodosCerradura,nodosAceptacion,'A'+k);
		//Se recorre el alfabeto y se aplica la operacion 'mover'
		for(int i=0;i<alfabeto.size();i++){
			nodosMover.clear();
			mover(nodosCerradura,nodosMover,alfabeto[i]);
			//Checa si el estado ya existe
			char est = no_marcado(nodosMover,tablaEstados,estadoN);
			if(est == estadoN){
				print(nodosMover,alfabeto[i],'A'+k,estadoN);
				meterEstado(nodosMover,tablaEstados,estadoN);
			}else printR(nodosMover,alfabeto[i],'A'+k,est);
		}
	}

	aceptacion(nodosAceptacion);
	printTablaEstados(tablaEstados);

	return 0;
}

void cerradura_e(vector<int> &nodosMover,vector<int> &nodosCerradura,vector<char> &nodosAceptacion,char estado){
	stack<char> pila;
	nodosCerradura.clear();

	//Para el primer estado
	if(nodosMover.empty())
		nodosMover.push_back(1);
	//
	for(int i=0;i<nodosMover.size();i++) {
		//Se mete a nodosCerradura y a la pila
		nodosCerradura.push_back(nodosMover[i]);
		pila.push(nodosMover[i]);
		//Si es nodo de aceptacion se mete en nodosAceptacion
		if(nodosMover[i] == ACEPTACION)
			nodosAceptacion.push_back(estado);
	}
	
	while(!pila.empty()) {
		int t = pila.top();
		pila.pop();
		for(auto tran: transiciones[t]){
			int i = 0;
			//Recorre nodosCerradura chechando si no esta el nodo
			while(i<nodosCerradura.size() and tran.first!=nodosCerradura[i])
				i++;
			if(i == nodosCerradura.size()){
				//Checha que tenga una transicion epsilon
				if(tran.second==EPSILON){
					//Se mete a nodosCerradura y a la pila
					nodosCerradura.push_back(tran.first);
					pila.push(tran.first);
					//Si es nodo de aceptacion se mete en nodosAceptacion
					if(tran.first == ACEPTACION)
						nodosAceptacion.push_back(estado);
				}
			}
		}
	}
	cout << "\n" << estado <<" : {|";
	for(int i=0;i<nodosMover.size();i++)
		cout<< nodosMover[i] << (i<nodosMover.size()-1? ",":"|,");

	for(int j=nodosMover.size();j<nodosCerradura.size();j++)
		cout << nodosCerradura[j] << (j<nodosCerradura.size()-1? ',':' ');
	cout << "}" << endl;
}

void meterEstado(vector<int> &nodosCerradura,vector<pair<char,vector<int>>> &tablaEstados,char &estadoN){
	//Al estado pozo se le va a meter un -1 para tener un mejor control
	if(!nodosCerradura.size())
		nodosCerradura.push_back(-1);
		
	tablaEstados.push_back(make_pair(estadoN,nodosCerradura));
	++estadoN;
}

void mover(vector<int> &nodosCerradura,vector<int> &nodosMover,int c){
	for(int i=0;i<nodosCerradura.size();i++){
		//Checha si las transiciones del nodo corresponden al valor de 'c'
		for(auto tran: transiciones[nodosCerradura[i]]){
			if(tran.second==c)
				nodosMover.push_back(tran.first);
		}
	}
	//Al estado pozo se le va a meter un -1 para tener un mejor control
	if(nodosMover.size() == 0) nodosMover.push_back(-1);
}

void print(vector<int> &nodosMover,char c,char estado, char estadoN){
	cout << " mover("<<estado<<","<< c <<") : {";
	for(int j=0;j<nodosMover.size();j++)
		cout << nodosMover[j] << (j<nodosMover.size()-1? ',':'}');
	cout << " =  " << estadoN << endl;
}

void printP(vector<int> &nodosMover,char c,char estado, char estadoN){
	cout << " mover("<<estado<<","<< c <<") : {} = " << estadoN <<" (estado pozo)" << endl;
}

void printR(vector<int> &nodosMover,char c,char estado, char estadoN){
	cout << " mover("<<estado<<","<< c <<") : " << estadoN << endl;
}

char no_marcado(vector<int> nodosMover,vector<pair<char,vector<int>>> tablaEstados, char estadoN){
	sort(nodosMover.begin(),nodosMover.end());
	for(int k=0;k<tablaEstados.size();k++) {
		sort(tablaEstados[k].second.begin(),tablaEstados[k].second.end());
		int i=0;
		for(int j=0;j<nodosMover.size();j++){
			if(nodosMover[j]==tablaEstados[k].second[j])
				i++;
		}
		if(i==nodosMover.size()) return tablaEstados[k].first;
	}
	return estadoN;
}

void aceptacion(vector<char> &nodosAceptacion){
	cout << "\nNodos aceptacion : {";
	for(auto acept: nodosAceptacion)
		cout<<acept<<" ";
	cout << "}" << endl;
}

void printTablaEstados(vector<pair<char,vector<int>>> &tablaEstados){
	for(int i=0;i<tablaEstados.size();i++){
		cout << tablaEstados[i].first << ": ";
		for(int j=0;j<tablaEstados[i].second.size();j++)
			cout << tablaEstados[i].second[j] << (j==tablaEstados[i].second.size()-1?' ':',');
		cout << endl;
	}
}
