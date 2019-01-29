#include <iostream>
#include <string>
#include <vector>
#include "funcoes.h"

using namespace std;


/**
* Confere a dependência dos resgistradores
* @param dependeciaregistador  
* @param Registrador2 
* @param Registrador3
* @param RegistradorAux 

* @return Quantas posições ele terá que pular
*/
int dependecia (const int &ciclo, vector <string> &dependeciaregistador, const string &Registrador2, const string &Registrador3, const string &RegistradorAux = "0")
{
  for(int i = 0; i < 4; i++)
  {
    if (ciclo == 0) 
      return ciclo;
    else if((dependeciaregistador[i].compare(Registrador2) == 0) || (dependeciaregistador[i].compare(Registrador3) == 0) || (dependeciaregistador[i].compare(RegistradorAux) == 0)) 
    {
      dependeciaregistador[i] = " ";
      return (ciclo - i + 4);
    }
  }
  return (ciclo + 1);
}

/**
* Separa cada integrante do arquivo e "chama as funções"
* @param estagioIF 
* @param estagioID 
* @param estagioEX
* @param estagioMEM 
* @param estagioWB 
*/
void pipeline (vector <string> &estagioIF, vector <string> &estagioID,  vector <string> &estagioEX, vector <string> &estagioMEM, vector <string> &estagioWB, vector <string> Instrucoes, int &ciclo)
{

  string instrucao, salto, operation, rest, Registrador1 = " ", Registrador2, Registrador3, RegistradorAux, nodependecia = " ";
  int vazios = Instrucoes.size();
  bool flagsalto = false;
  vector <string> dependeciaregistador (4);

  for(int i = 0; i < vazios; i++, ciclo++) 
  {
	size_t found;
	instrucao = Instrucoes[i];
	found = instrucao.find_first_of(":");

	if (found!=string::npos)
	{
		operation = instrucao.substr(0,found);
	}
	else
	{
		found = instrucao.find_first_of(" ");
		operation = instrucao.substr(0,found);
		rest = instrucao.substr(found + 1);
	}

    if (operation.compare(salto) == 0)
    {
      flagsalto = false;
    }
    else if ((operation.compare("add") == 0 || operation.compare("sub") == 0) && flagsalto == false) 
    {
      found = rest.find_last_of(" ");
      Registrador3 = rest.substr(found+1);
      rest = rest.substr(0, found-1);
      found = rest.find_last_of(" ");
      Registrador2 = rest.substr(found+1);
      RegistradorAux = rest.substr(0,found-1);
      ciclo = dependecia(ciclo, dependeciaregistador, Registrador2, Registrador3, RegistradorAux);
      estagioIF[ciclo - i] = instrucao;
	  estagioID[ciclo + 1 - i] = instrucao;
	  estagioEX[ciclo + 2 - i] = instrucao;
	  estagioMEM[ciclo + 3 - i] = instrucao;
	  estagioWB[ciclo + 4 - i] = instrucao;
      Registrador1 = RegistradorAux;
    }
    else if ((operation.compare("beq") == 0 || operation.compare("bne") == 0) && flagsalto == false) 
    {
      found = rest.find_first_of(",");
      Registrador2 = rest.substr(0, found);
      rest = rest.substr(found+2);
      found = rest.find_first_of(",");
      Registrador3 = rest.substr(0, found);
      salto = rest.substr(found+2);
      ciclo = dependecia (ciclo, dependeciaregistador, Registrador2, Registrador3);
      estagioIF[ciclo - i] = instrucao;
	  estagioID[ciclo + 1 - i] = instrucao;
	  estagioEX[ciclo + 2 - i] = instrucao;
	  estagioMEM[ciclo + 3 - i] = instrucao;
	  estagioWB[ciclo + 4 - i] = instrucao;
      flagsalto = true;
      Registrador1 = nodependecia;
    }
    else if (operation.compare("lw") == 0 && flagsalto == false)
    {
      found = rest.find_first_of(",");
      Registrador2 = rest.substr(0, found);
      rest = rest.substr(found+2);
      found = rest.find_first_of("(");
      rest = rest.substr(found+1);
      Registrador3 = rest.substr(0, rest.size() - 1);
      ciclo = dependecia (ciclo, dependeciaregistador, Registrador2, Registrador3);
      
      estagioIF[ciclo - i] = instrucao;
	  estagioID[ciclo + 1 - i] = instrucao;
	  estagioEX[ciclo + 2 - i] = instrucao;
	  estagioMEM[ciclo + 3 - i] = instrucao;
	  estagioWB[ciclo + 4 - i] = instrucao;
      Registrador1 = Registrador2;
    }
    else if (operation.compare("sw") == 0 && flagsalto == false)
    {
      found = rest.find_first_of(",");
      Registrador2 = rest.substr(0, found);
      rest = rest.substr(found+2);
      found = rest.find_first_of("(");
      rest = rest.substr(found+1);
      Registrador3 = rest.substr(0, rest.size() - 1);
      ciclo = dependecia (ciclo, dependeciaregistador, Registrador2, Registrador3);
      estagioIF[ciclo - i] = instrucao;
	  estagioID[ciclo + 1 - i] = instrucao;
	  estagioEX[ciclo + 2 - i] = instrucao;
	  estagioMEM[ciclo + 3 - i] = instrucao;
	  estagioWB[ciclo + 4 - i] = instrucao;      
      Registrador1 = nodependecia;
    }
    else if (operation.compare("j") == 0 && flagsalto == false)
    {
      salto = rest;
      if (ciclo != 0) ciclo++;
      estagioIF[ciclo - i] = instrucao;
	  estagioID[ciclo + 1 - i] = instrucao;
	  estagioEX[ciclo + 2 - i] = instrucao;
	  estagioMEM[ciclo + 3 - i] = instrucao;
	  estagioWB[ciclo + 4 - i] = instrucao;     
      flagsalto = true;
      Registrador1 = nodependecia;
    }
    
    for(int j = 3; j > 0; j--)
 	{
  		dependeciaregistador[j] = dependeciaregistador[j-1];
 	}
  	dependeciaregistador[0] = Registrador1;
  }
  ciclo = ciclo - vazios + 5;
}