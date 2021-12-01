class PseudoAF{
private:
	int inicial;
	int final;
public:
	PseudoAF(int inicial, int final){
		this->inicial = inicial;
		this->final = final;
	}
	PseudoAF(){
		inicial = 0;
		final = 0;
	}
	int ini(){
		return inicial;
	}
	int fin(){
		return final;
	}
	void cambiar_ini(int inicial){
		this->inicial = inicial;
	}
	void cambiar_fin(int final){
		this->final = final;
	}
};