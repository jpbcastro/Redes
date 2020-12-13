#include <string>
#include <bitset>
#include <iostream>
#include <vector>
#include <array>
using namespace std;

typedef struct bloco{
	bitset<8> byte;
	int paridade;
	bitset<8> R;
}Bloco;
void AplicacaoTransmissora();
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaEnlaceDadosTransmissora(vector<bitset<8>> quadro);

//void MeioDeComunicacao(bloco fluxoDeBits[]);
//void CamadaEnlaceDadosReceptora(FluxoBrutoDeBits[])

void CamadaDeAplicacaoReceptora(vector<bitset<8>> quadro);
void AplicacaoReceptora(vector<char> mensagem);
Bloco * CamadaEnlaceDadosTransmissoraControleDeErroCRC(Bloco * fluxoDeBits, int maxSize);
Bloco * CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(Bloco * fluxoDeBits, int maxSize);
Bloco * CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(Bloco * fluxoDeBits, int maxSize);
vector<bitset<8>> chartobin(string mensagem);
vector<char> bintochar(vector<bitset<8>> binarios);

int main(){
	AplicacaoTransmissora();
	return 0;
}

void AplicacaoTransmissora(void){
	string mensagem;
	cout<<"Digite uma mensagem:"<<endl;
	cin>>mensagem;
	CamadaDeAplicacaoTransmissora(mensagem);
}

void CamadaDeAplicacaoTransmissora(string mensagem){
	vector<bitset<8>> quadro; 
	quadro = chartobin(mensagem);
	CamadaEnlaceDadosTransmissora(quadro);
}

void CamadaEnlaceDadosTransmissora(vector<bitset<8>> quadro){
	int TipoDeControleDeErro = 2;
	auto n = quadro.size();
	Bloco * fluxoDeBits;
	int i, j;
	int cont=0;

	fluxoDeBits = (Bloco*) malloc(n*sizeof(Bloco));

	for(i=0; i<n; i++){
				fluxoDeBits[i].byte = quadro[i];
	}

	switch(TipoDeControleDeErro){
		case 0:
			fluxoDeBits = CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(fluxoDeBits, n);
			/*
			for(i=0; i<n; i++){
				fluxoDeBits[i].byte = quadro[i];
				for(j=0; j<8; j++){
					if(fluxoDeBits[i].byte[j]==1){
						cont++;
					}
				}
				if(cont%2==0){
					fluxoDeBits[i].paridade=0;
				}
				else{
					fluxoDeBits[i].paridade=1;
				}
			}*/
			break;
		case 1:
			fluxoDeBits = CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(fluxoDeBits, n);
		/*
			for(i=0; i<n; i++){
				fluxoDeBits[i].byte = quadro[i];
				for(j=0; j<8; j++){
					if(fluxoDeBits[i].byte[j]==1){
						cont++;
					}
				}
				if(cont%2==0){
					fluxoDeBits[i].paridade=1;
				}
				else{
					fluxoDeBits[i].paridade=0;
				}
			}*/
			break;
		
		case 2:
			fluxoDeBits = CamadaEnlaceDadosTransmissoraControleDeErroCRC(fluxoDeBits, n);

			break;
	}
	//MeioDeComunicacao(fluxoDeBits);
}
/*
void MeioDeComunicacao(bloco fluxoDeBits[]){
	int erro, porcentagemDeErros;
	int fluxoBrutoDeBitsPontoA[], fluxoBrutoDeBitsPontoB[];
	porcentagemDeErros=0;
	fluxoBrutoDeBitsPontoA=fluxoBrutoDeBits;
	while(fluxoBrutoDeBitsPontoB.lenght()!=fluxoBrutoDeBitsPontoA.lenght()){
		if((rand()%100)==a){
			fluxoBrutoDeBitsPontoB += fluxoBrutoDeBitsPontoA;
		}
		else{
			fluxoBrutoDeBitsPontoB;
		}
	}

	CamadaEnlaceDadosReceptora();
}

void CamadaEnlaceDadosReceptora(FluxoBrutoDeBits[]){
	//
	CamadaDeAplicacaoReceptora(quadro);
}*/

void CamadaDeAplicacaoReceptora(vector<bitset<8>> quadro){
	vector<char> mensagem;
	mensagem = bintochar(quadro);
	AplicacaoReceptora(mensagem);
}

void AplicacaoReceptora(vector<char> mensagem){
	cout << "A mensagem recebida foi:";
	int i;
	auto n = mensagem.size();
	for(i=0; i<n; i++){
    	cout << mensagem[i];
    }
}

vector<bitset<8>> chartobin(string mensagem){
	int n = mensagem.length();
	int numero[n];
	vector<bitset<8>> binarios;
	bitset<8> byte;
	int i;
	for(i=0; i<n; i++){
		numero[i]=mensagem[i];
		byte = numero[i];
		binarios.push_back(byte);
	}
	return binarios;
}

vector<char> bintochar(vector<bitset<8>> binarios){
	auto n = binarios.size();
	vector<int> numbers;
	int numbaux, i;
	for(i=0; i<n; i++){
		numbaux = (int)(binarios[i].to_ulong());
		numbers.push_back(numbaux);
	}
	vector<char> chars{};
    for (auto &number : numbers) {
        chars.push_back(number);
    }
    return chars;
}

Bloco * CamadaEnlaceDadosTransmissoraControleDeErroCRC(Bloco * fluxoDeBits, int maxSize){
	int i, j;

	for(i = 0; i<maxSize; i++){
		for
	}

	return fluxoDeBits;
}

Bloco * CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(Bloco * fluxoDeBits, int maxSize){
	int i, j;
	int cont=0;

	for(i=0; i<maxSize; i++){
				for(j=0; j<8; j++){
					if(fluxoDeBits[i].byte[j]==1){
						cont++;
					}
				}
				if(cont%2==0){
					fluxoDeBits[i].paridade=0;
				}
				else{
					fluxoDeBits[i].paridade=1;
				}
	}

	return fluxoDeBits;
}

Bloco * CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(Bloco * fluxoDeBits, int maxSize){
	int i, j;
	int cont=0;

	for(i=0; i<maxSize; i++){
				for(j=0; j<8; j++){
					if(fluxoDeBits[i].byte[j]==1){
						cont++;
					}
				}
				if(cont%2==0){
					fluxoDeBits[i].paridade=1;
				}
				else{
					fluxoDeBits[i].paridade=0;
				}
	}

	return fluxoDeBits;
}