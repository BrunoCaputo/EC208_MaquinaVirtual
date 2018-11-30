/*
	PROJETO DE ARQUITETURA DE COMPUTADORES II

	Aluno: Bruno Pereira Garcia Caputo

	Detalhes do SET:

	Tamanho das instruções: 16 bits.

	Código:
	ADD:	0001
	SUB:	0010
	DIV:	0011
	LOAD:	0100
	STORE:	1000

	ADD/SUB/DIV
	(Tipo instr.)	(End. Reg 1)	(End. Reg 2)	(End. Reg Dest.)
    	4 bits		   4 bits		   4 bits			 4 bits

 	LOAD/STORE
 	(Tipo instr.)	(End. Reg)	(Memória de dados)
 	   4 bits		  4 bits		 8 bits
		
    Cache:
    
	4 words por linha
	Tag = 16 - (4(log2 nº linhas) + 2(log2 nº words) + 0(Byte)) = 10bits
    
	(Tag)	(Nºlinha)	(Nº word)
   10 bits	 4 bits	     2 bits
   
	Total = 32 * ((16*4) + 10  + 1) = 2400bits = 300bytes
*/
#include <bits/stdc++.h>

using namespace std;

//cache
typedef struct cache{
	unsigned int words[4];		//Quantidade de words/linha
	unsigned int tag;			//Tag da linha
	bool valid;					//Bit de validação
}cacheStruct;

//protótipos
void execute();
void decode();
unsigned int buscaCache(unsigned int PCcache);
void veCache();

//Variáveis de controle
int div0;

//Memória de programa
unsigned int progMem[] = {0b0100000000000000,
						  0b0100000100000001,
						  0b0010000000010010,
						  0b1000001000000010,
						  0b0100001000000000,
						  0b0100001100000001,
						  0b0001001000110100,
						  0b1000010000000100};

//Memória de Dados
unsigned int dataMem[] = {10, 20, 2, 0, 6, 80, 50};

//Registradores
unsigned int PC;					//Contador de programa
unsigned int instrucao;				//Instrução com os 16 bits
unsigned int tipoInstrucao;			//ADD, SUB, DIV, LOAD, STORE
unsigned int reg1;					//Registrador com o valor 1
unsigned int reg2;					//Registrador com o valor 2
unsigned int regDest;				//Registrador com o resultado de 1 e 2
unsigned int regEndMem;				//Registrador do endereço de memória
cacheStruct L1[16];					//Memória cache L1
int reg[16];						//Registradores para armazenamento dos dados

int main(){
	PC = 0;
	div0 = 0;

	for(int i = 0; i < 16; i++){
		reg[i] = 0;
	}

	while(PC < 8){
		instrucao = buscaCache(PC);
		decode();
		execute();
		PC++;
	}
	
	veCache();

	return 0;
}

//Função de decodificação
void decode(){
	tipoInstrucao = instrucao >> 12;
	
	//ADD, SUB, DIV
	if(tipoInstrucao == 1 || tipoInstrucao == 2 || tipoInstrucao == 3){
		reg1 = instrucao >> 8;
		reg1 = reg1 & 0b0000000000001111;
		reg2 = instrucao >> 4;
		reg2 = reg2 & 0b0000000000001111;
		regDest = instrucao & 0b0000000000001111;
	}
	//LOAD
	else if(tipoInstrucao == 4){
		regDest = instrucao >> 8;
		regDest = regDest & 0b0000000000001111; 
		regEndMem = instrucao & 0b0000000011111111;
	}
	//STORE
	else if(tipoInstrucao == 8){
		reg1 = instrucao >> 8;
		reg1 = reg1 & 0b0000000000001111; 
		regEndMem = instrucao & 0b0000000011111111;
	}
}

void execute(){
	//ADD
	if(tipoInstrucao == 1){
		reg[regDest] = reg[reg1] + reg[reg2];
		cout << "Somando: " << reg[reg1] << " + " << reg[reg2];
		cout << " Resultado: " << reg[regDest] << endl;
	}
	//SUB
	else if(tipoInstrucao == 2){
		reg[regDest] = reg[reg1] - reg[reg2];
		cout << "Subtraindo: " << reg[reg1] << " - " << reg[reg2];
		cout << " Resultado: " << reg[regDest] << endl;
	}
	//DIV
	else if(tipoInstrucao == 3){
		if(reg[reg2] == 0){
			printf("Nao existe divisao por 0\n");
			div0 = 1;
		}
		else{
			reg[regDest] = reg[reg1] / reg[reg2];
			cout << "Dividindo: " << reg[reg1] << " / " << reg[reg2];
			cout << " Resultado: " << reg[regDest] << endl;
		}
	}
	//LOAD
	else if(tipoInstrucao == 4){
		reg[regDest] = dataMem[regEndMem];
		cout << "Carregando " << dataMem[regEndMem] << " no registrador" << endl;
	}
	// STORE
	else if(tipoInstrucao == 8){
		if(div0 == 1)
			return;
		dataMem[regEndMem] = reg[reg1];
		cout << "Salvando " << reg[reg1] << " na memoria de dados" << endl;
	}
}

unsigned int buscaCache(unsigned int PCcache){
	unsigned int word = PCcache & 0b0000000000000011;
	unsigned int line = PCcache & 0b0000000000111100;
	unsigned int tag = PCcache & 0b1111111111000000;
	
	if(!L1[line].valid || L1[line].tag != tag){
		printf("MISS\n");
		L1[line].valid = true;
		L1[line].tag = tag;
		
		for(int i = 0; i < 4; i++){
			L1[line].words[i] = progMem[PCcache + i];
		}
	}
	
	return L1[line].words[word];
}

void veCache(){
	cout << endl;
	for(int i = 0; i < 16; i++){
		if(L1[i].valid){
			cout << "Tag: " << L1[i].tag << endl;
			for(int j = 0; j < 4; j++){
				cout << "Word " << j + 1 << ": " << L1[i].words[j] << endl;
			}
		}
	}
}























