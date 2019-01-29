#include <iostream>
#include <string>
#include <vector>
#include "funcoes.h"

using namespace std;

int main(int argc, char * argv[]) 
{
  
  //Vector de strings para alocar instruções
	vector <string> Instrucoes;

  string instrucao;
  while ( getline(cin, instrucao) ) 
  {
    Instrucoes.push_back(instrucao);
  }

  //Insere 0 em cada estágio, caso eles estejam vazios (nenhuma instrução estiver nele)
	int ciclo = 0, vazios = 1 + 4*(Instrucoes.size());

  vector <string> estagioIF, estagioID, estagioEX, estagioMEM, estagioWB;

  for (int i=0; i<vazios; i++)
  {
    estagioIF.push_back("0");
    estagioID.push_back("0");
    estagioEX.push_back("0");
    estagioMEM.push_back("0");
    estagioWB.push_back("0");
  }
  
	pipeline(estagioIF, estagioID, estagioEX, estagioMEM, estagioWB, Instrucoes, ciclo);
	
  cout << "Quantidade de ciclos total: " <<  ciclo << endl;
  cout << endl << "-------------------------------------------------------" << endl;
  for(int i = 0; i < ciclo; i++){
    cout << "Ciclo " << (i + 1) << endl;
    cout << "IF:" << "\t" << estagioIF[i] << endl;
    cout << "ID:" << "\t" << estagioID[i] << endl;
    cout << "EX:" << "\t" << estagioEX[i] << endl;
    cout << "MEM:" << "\t" << estagioMEM[i] << endl;
    cout << "WB:" << "\t" <<  estagioWB[i]<< endl;
    cout << "-------------------------------------------------------" << endl;
  }

	return 0;
	}
