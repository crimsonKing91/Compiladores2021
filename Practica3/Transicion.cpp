class Transicion{
public:
	int estado_objetivo;
	char simbolo;

	Transicion(){
		estado_objetivo = 0;
		simbolo = 'E';
	}
	Transicion(int estado_objetivo, char simbolo){
		this->estado_objetivo = estado_objetivo;
		if (simbolo == 'E' || (simbolo >= 'a' && simbolo <= 'z'))
			this->simbolo = simbolo;
		else throw "El símbolo no forma parte del conjunto permitido de simbolos"; //Arrojar un error si el simbolo no es parte del conjunto
	}
};