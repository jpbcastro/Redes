#include <string>
#include <bitset>
#include <iostream>
#include <vector>
#include <array>
using namespace std;

struct bloco{
	bitset<8> byte;
	int paridade;
	bitset<32> R;
};

int TipoDeControleDeErro = 2;

void AplicacaoTransmissora();
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaEnlaceDadosTransmissora(vector<bitset<8>> quadro);

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector<bitset<8>> quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(vector<bitset<8>> quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<bitset<8>> quadro);

void MeioDeComunicacao(bloco *fluxoDeBits, int tamanho);
void CamadaEnlaceDadosReceptora(bloco *fluxoDeBits, int tamanho);

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(bloco *fluxoDeBits, int tamanho);
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(bloco *fluxoDeBits, int tamanho);
void CamadaEnlaceDadosReceptoraControleDeErroCRC(bloco *fluxoDeBits, int tamanho);

void CamadaDeAplicacaoReceptora(bloco *fluxoDeBits, int tamanho);
void AplicacaoReceptora(vector<char> mensagem);

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
	switch(TipoDeControleDeErro){
		case 0:
			CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(quadro);
			break;
		case 1:
			CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(quadro);
			break;
		case 2:
			CamadaEnlaceDadosTransmissoraControleDeErroCRC(quadro);
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector<bitset<8>> quadro){
	auto n = quadro.size();
	bloco fluxoDeBits[n];
	int i, j;
	int cont;
	for(i=0; i<n; i++){
		cont=0;
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
	}
	MeioDeComunicacao(fluxoDeBits, n);
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(vector<bitset<8>> quadro){
	auto n = quadro.size();
	bloco fluxoDeBits[n];
	int i, j;
	int cont;
	for(i=0; i<n; i++){
		cont=0;
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
	}
	MeioDeComunicacao(fluxoDeBits, n);
}

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<bitset<8>> quadro){
	auto n = quadro.size();
	bloco fluxoDeBits[n];
	int numbaux, r;
	int i, j;
	for(i=0; i<n; i++){
		fluxoDeBits[i].byte = quadro[i];
		numbaux = (int)(quadro[i].to_ulong());
		r = numbaux % (2^33+0x04C11DB7);
		fluxoDeBits[i].R = r;

	}
	MeioDeComunicacao(fluxoDeBits, n);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void MeioDeComunicacao(bloco *fluxoDeBits, int tamanho){
	int i, j, porcentagemDeErros;
	porcentagemDeErros = 0;

	cout << "enviado:\n";
	for(i=0; i<tamanho; i++){
		cout << fluxoDeBits[i].byte << " paridade " << fluxoDeBits[i].paridade << "\n";
	}

	for(i=0; i<tamanho; i++){
		for(j=0; j<8; j++){
			if((rand()%100)<porcentagemDeErros){
				fluxoDeBits[i].byte[j]==0?fluxoDeBits[i].byte[j]=true:fluxoDeBits[i].byte[j]=false;
			}
		}
	}

	cout << "recebido:\n";
	for(i=0; i<tamanho; i++){
		cout << fluxoDeBits[i].byte << " paridade " << fluxoDeBits[i].paridade << "\n";
	}

	CamadaEnlaceDadosReceptora(fluxoDeBits, tamanho);
}

void CamadaEnlaceDadosReceptora(bloco *fluxoDeBits, int tamanho){
	switch(TipoDeControleDeErro){
		case 0:
			CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(fluxoDeBits, tamanho);
			break;
		case 1:
			CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(fluxoDeBits, tamanho);
			break;
		case 2:
			CamadaEnlaceDadosReceptoraControleDeErroCRC(fluxoDeBits, tamanho);
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(bloco *fluxoDeBits, int tamanho){
	bitset<8> byteaux (string("00100011"));
	int i, j, paridaux;
	int cont;
	for(i=0; i<tamanho; i++){
		cont=0;
		for(j=0; j<8; j++){
			if(fluxoDeBits[i].byte[j]==1){
				cont++;
			}
		}
		if(cont%2==0){
			paridaux=0;
		}
		else{
			paridaux=1;
		}
		if(paridaux!=fluxoDeBits[i].paridade){
			fluxoDeBits[i].byte=byteaux;
		}
	}
	CamadaDeAplicacaoReceptora(fluxoDeBits, tamanho);
}

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(bloco *fluxoDeBits, int tamanho){
	bitset<8> byteaux (string("00100011"));
	int i, j, paridaux;
	int cont;
	for(i=0; i<tamanho; i++){
		cont=0;
		for(j=0; j<8; j++){
			if(fluxoDeBits[i].byte[j]==1){
				cont++;
			}
		}
		if(cont%2==0){
			paridaux=1;
		}
		else{
			paridaux=0;
		}
		if(paridaux!=fluxoDeBits[i].paridade){
			fluxoDeBits[i].byte=byteaux;
		}
	}
	CamadaDeAplicacaoReceptora(fluxoDeBits, tamanho);
}

void CamadaEnlaceDadosReceptoraControleDeErroCRC(bloco *fluxoDeBits, int tamanho){
	bitset<8> byteaux (string("00100011"));
	int i, r, d;

	for(i=0; i<tamanho; i++){
		r = (int)(fluxoDeBits[i].R.to_ulong());
		d = (int)(fluxoDeBits[i].byte.to_ulong());
		if((2^33+0x04C11DB7)%(d+r)!=0){
			fluxoDeBits[i].byte=byteaux;
		}

	}

	CamadaDeAplicacaoReceptora(fluxoDeBits, tamanho);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CamadaDeAplicacaoReceptora(bloco *fluxoDeBits, int tamanho){
	vector<bitset<8>> quadro;
	int i;
	for(i=0; i<tamanho; i++){
		quadro.push_back(fluxoDeBits[i].byte);
	}
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