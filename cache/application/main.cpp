#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include "cache.h"

int main(int argc, char * argv[]) 
{
  std::ifstream inputFile; //Arquivo de entrada
  inputFile.open("config.txt");

  //Entrada dos parâmetros de configuração
  Memoria <int> inputMemoria; //dados da memoria
  Cache <int> inputCache; //dados da cache
  inputFile >> inputMemoria.blocos >> inputCache.linhas >> inputMemoria.sizeMemoria >> inputMemoria.mapeamento >> inputMemoria.sizeconjuntos >> inputMemoria.politicaSubs;// separação dos dados na suas respectivas funcoes
  inputCache.blocos = inputMemoria.blocos; 

  //Criação dos objetos baseados nos parâmetros de entrada
  inputMemoria.criarMemoria(inputMemoria); ///cria memoria principal
  inputCache.criarCache(inputCache); // cria cache

  //Variáveis 
  std::string instrucao; //instrucao - > read show
  std::size_t found;
  int palavra = 0, blocoCache = 0, blocoMemoria = 0, rest = 0, sizeWay = 0,blocoWay = 0, inicio = 0, fim = 0, blocoCacheAux=0; //palavra numero a ser buscado, rest : resto do modulo, way modo, inicio, fim , aux
  bool estado = false; 

  //Contadores para as políticas de substituição LFU e LRU. 
  std::vector<int> blocoCacheconjuntos (inputMemoria.sizeconjuntos, 0);

  //Escolha de mapeamento
  switch (inputMemoria.mapeamento) //tipo de mapeamento
  {
    case 1:// DIRETO
      while (std::getline(std::cin, instrucao) && instrucao != "exit") //le do terminal
      {
        //Acha o espaço
        found = instrucao.find(" ");
        //Separa a palavra do dado
        if (found!=std::string::npos)
        {
          palavra = std::stoi(instrucao.substr(found + 1));
          instrucao = instrucao.substr(0,found);
        }
        // Se a função for read, executar as instruções abaixo
        if (instrucao.compare("read") == 0)
        {
          blocoCache = (palavra/inputMemoria.blocos)%inputCache.linhas;//a palavra vinda do terminal/pela quantidade d blocos da memoria principal % com as linhas da cache
          blocoMemoria = palavra/inputMemoria.blocos;
          rest = palavra%inputMemoria.blocos; //palavra vinda do terminal/quant d blocos vindos do memoria principal
          if(inputCache.elementos[blocoCache][rest] == palavra)
          {
            std::cout << "HIT linha " << blocoCache << std::endl;
          }
          else
          {
            std::cout << "MISS -> alocado na linha " << blocoCache << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
            mudarblocos(inputMemoria, inputCache, blocoCache, blocoMemoria);
          }
        }
        // Se a função for show
        else if (instrucao.compare("show") == 0)
        {
          inputCache.imprimirCache(inputCache, inputMemoria);
          inputMemoria.imprimirMemoria(inputMemoria);
        }
      }
      break;
    case 2://TOTALMENTE ASSOCIATIVO
      //Sabido o mapeamento e a política de substituição
      switch (inputMemoria.politicaSubs) 
      {
        //ALEATÓRIO
        case 1:
          while (std::getline (std::cin, instrucao) && instrucao != "exit") 
          {
            //Variáveis 
            inicio = 0;
            fim = inputCache.elementos.size();
            
            found = instrucao.find(" ");
            
            if (found!=std::string::npos)
            {
              palavra = std::stoi(instrucao.substr(found + 1));
              instrucao = instrucao.substr(0,found);
            }
            blocoMemoria = palavra/inputMemoria.blocos;
            
            if (instrucao.compare("read") == 0)
            {
              // Procurar dentro da cache
              estado = findpalavra(inputCache, palavra, blocoCache, inicio, fim);
              if(estado == true)
              {
                std::cout << "HIT linha " << blocoCache << std::endl;
                estado = false;
              }
              else
              {
                //Espaço vazio 
                estado = findpalavra(inputCache, -1, blocoCache, inicio, fim);
                if(estado == true)
                {
                  mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                }
                else
                {
                  // Caso não encontre, substitua uma palavra da cache baseado na política de substituição
                  srand (time(NULL));
                  blocoCache = rand()%inputCache.linhas;
                  mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                }
                std::cout << "MISS -> alocado na linha " << blocoCache << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
              }
            }
            else if (instrucao.compare("show") == 0)
            {
              inputCache.imprimirCache(inputCache, inputMemoria);
              inputMemoria.imprimirMemoria(inputMemoria);
            }
          }
          break;
        //FIFO
        case 2:
          while (std::getline (std::cin, instrucao) && instrucao != "exit") 
          {
            //Variáveis
            inicio = 0;
            fim = inputCache.elementos.size();
            
            found = instrucao.find(" ");
           
            if (found!=std::string::npos)
            {
              palavra = std::stoi(instrucao.substr(found + 1));
              instrucao = instrucao.substr(0,found);
            }
            blocoMemoria = palavra/inputMemoria.blocos;
            
            if (instrucao.compare("read") == 0)
            {
              //Procurar dentro da cache
              estado = findpalavra(inputCache, palavra, blocoCache, inicio, fim);
              if(estado == true)
              {
                std::cout << "HIT linha " << blocoCache << std::endl;
                estado = false;
              }
              else
              {
                //Caso não encontre, substitua uma palavra da cache baseado na política de substituição. 
                mudarblocos (inputMemoria, inputCache, blocoCacheAux%inputCache.linhas, blocoMemoria);
                std::cout << "MISS -> alocado na linha " << (blocoCacheAux%inputCache.linhas) << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
                blocoCacheAux = blocoCacheAux + 1;
              }
            }
            else if (instrucao.compare("show") == 0)
            {
              inputCache.imprimirCache(inputCache, inputMemoria);
              inputMemoria.imprimirMemoria(inputMemoria);
            }
          }
          break;
        // LFU
        case 3:
          //Contadores para a política de substituição LFU
          for(int i = 0; i < inputCache.linhas; i++)
          {
          	inputCache.cont.push_back(0);
          } 
          while (std::getline (std::cin, instrucao) && instrucao != "exit") 
          {
            inicio = 0;
            fim = inputCache.elementos.size();
           
            found = instrucao.find(" ");

            if (found!=std::string::npos)
            {
              palavra = std::stoi(instrucao.substr(found + 1));
              instrucao = instrucao.substr(0,found);
            }
            blocoMemoria = palavra/inputMemoria.blocos;
            // Se a função for read, execute as instruções abaixo
            if (instrucao.compare("read") == 0)
            {
              // Procurar dentro da cache
              estado = findpalavra (inputCache, palavra, blocoCache, inicio, fim);
              if(estado == true)
              {
                std::cout << "HIT linha " << blocoCache << std::endl;
                inputCache.cont[blocoCache] = inputCache.cont[blocoCache] + 1;
                estado = false;
              }
              else
              {
                estado = findpalavra (inputCache, -1, blocoCache, inicio, fim);
                if(estado == true)
                {
                  //Procura espaço na cache 
                  mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                  inputCache.cont[blocoCache] = 1;
                }
                else
                {
                  //Subst. a palavra de acordo com o modo de substituicao 
                  blocoCache = inputCache.findMenosUtilizadoscomFrequencia(inputCache);
                  mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                  inputCache.cont[blocoCache] = 1;
                }
                std::cout << "MISS -> alocado na linha " << blocoCache << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
              }
            }
            //Show 
            else if (instrucao.compare("show") == 0)
            {
              inputCache.imprimirCache(inputCache, inputMemoria);
              inputMemoria.imprimirMemoria(inputMemoria);
            }
          }
          break;
        // LRU. 
        case 4:
           
          for(int i = 0; i < inputCache.linhas; i++) 
          {
            inputCache.cont.push_back(0);
          }
          while (std::getline (std::cin, instrucao) && instrucao != "exit") 
          {
            // Inicialização do variáveis necessárias no programa. 
            inicio = 0;
            fim = inputCache.elementos.size();
            
            found = instrucao.find(" ");
            //Divide  
            if (found!=std::string::npos)
            {
              palavra = std::stoi(instrucao.substr(found + 1));
              instrucao = instrucao.substr(0,found);
            }
            blocoMemoria = palavra/inputMemoria.blocos;
            // read
            if (instrucao.compare("read") == 0)
            {
              // Procurar dentro da cache. 
              estado = findpalavra (inputCache, palavra, blocoCache, inicio, fim);
              if(estado == true)
              {
                std::cout << "HIT linha " << blocoCache << std::endl;
                inputCache.cont[blocoCache] = -1;
                estado = false;
              }
              else
              {
                //Procura espaço na cache 
                estado = findpalavra (inputCache, -1, blocoCache, inicio, fim);
                if(estado == true)
                {
                  mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                  inputCache.cont[blocoCache] = -1;
                }
                else
                {
                  //Subst. a palavra de acordo com o modo de substituicao 
                  blocoCache = inputCache.findMenosRecenteUsado(inputCache);
                  mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                  inputCache.cont[blocoCache] = -1;
                }
                std::cout << "MISS -> alocado na linha " << blocoCache << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
              }
              inputCache.mudarMenosRecenteUsado(inputCache);
            }
            //Show 
            else if (instrucao.compare("show") == 0)
            {
              inputCache.imprimirCache(inputCache, inputMemoria);
              inputMemoria.imprimirMemoria(inputMemoria);
            }
          }
          break;
      }
      break;
    // PARCIALMENTE ASSOCIATIVO. 
    case 3:
    switch (inputMemoria.politicaSubs)
    {
      // ALEATÓRIO
      case 1:
        while (std::getline (std::cin, instrucao) && instrucao != "exit") 
        {
          
          found = instrucao.find(" ");
          //Divide  
          if (found!=std::string::npos)
          {
            palavra = std::stoi(instrucao.substr(found + 1));
            instrucao = instrucao.substr(0,found);
          }
          // Na parcialmente associativa o intervalo é definido onde a palavra sera inserida de acordo com o bloco
          blocoMemoria = palavra/inputMemoria.blocos;
          sizeWay = inputCache.linhas/inputMemoria.sizeconjuntos;
          blocoWay = blocoMemoria%inputMemoria.sizeconjuntos;
          inicio = (blocoWay*sizeWay);
          fim = inicio + sizeWay;

          // read
          if (instrucao.compare("read") == 0)
          {
            // Procurar dentro da cache. 
            estado = findpalavra (inputCache, palavra, blocoCache, inicio, fim);
            if(estado == true)
            {
              std::cout << "HIT linha " << blocoCache << std::endl;
              estado = false;
            }
            else
            {
              //Procura espaço na cache 
              estado = findpalavra (inputCache, -1, blocoCache, inicio, fim);
              if(estado == true)
              {
                mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                estado = false;
              }
              else
              {
                //Subst. a palavra de acordo com o modo de substituicao 
                srand (time(NULL));
                blocoCache = inicio + rand() % sizeWay;
                mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
              }
              std::cout << "MISS -> alocado na linha " << blocoCache << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
            }
          }
          //Show 
          else if (instrucao.compare("show") == 0)
          {
            inputCache.imprimirCache(inputCache, inputMemoria);
            inputMemoria.imprimirMemoria(inputMemoria);
          }
        }
        break;
      // FIFO. 
      case 2:
        while (std::getline (std::cin, instrucao) && instrucao != "exit") 
        {
          
          found = instrucao.find(" ");
          //Divide  
          if (found!=std::string::npos)
          {
            palavra = std::stoi(instrucao.substr(found + 1));
            instrucao = instrucao.substr(0,found);
          }

          // Na parcialmente associativa o intervalo é definido onde a palavra sera inserida de acordo com o bloco 
          blocoMemoria = palavra/inputMemoria.blocos;
          sizeWay = inputCache.linhas/inputMemoria.sizeconjuntos;
          blocoWay = blocoMemoria%inputMemoria.sizeconjuntos;
          inicio = (blocoWay*sizeWay);
          fim = inicio + sizeWay;

          // read
          if (instrucao.compare("read") == 0)
          {
            // Procurar dentro da cache. 
            estado = findpalavra (inputCache, palavra, blocoCache, inicio, fim);
            if(estado == true)
            {
              std::cout << "HIT linha " << blocoCache << std::endl;
              estado = false;
            }
            else
            { 
              mudarblocos (inputMemoria, inputCache, (inicio + blocoCacheconjuntos[blocoWay]%sizeWay), blocoMemoria);
              blocoCacheconjuntos[blocoWay] = blocoCacheconjuntos[blocoWay] + 1;
              std::cout << "MISS -> alocado na linha " << (inicio + (blocoCacheconjuntos[blocoWay])%sizeWay) << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
            }
          }
          // Show 
          else if (instrucao.compare("show") == 0)
          {
            inputCache.imprimirCache(inputCache, inputMemoria);
            inputMemoria.imprimirMemoria(inputMemoria);
          }
        }
        break;
      //LFU. 
      case 3:
         
        for(int i = 0; i < inputCache.linhas; i++)
        {
          inputCache.cont.push_back(0);
        } 
        while (std::getline (std::cin, instrucao) && instrucao != "exit") 
        {
           
          found = instrucao.find(" ");
          
          if (found!=std::string::npos)
          {
            palavra = std::stoi(instrucao.substr(found + 1));
            instrucao = instrucao.substr(0,found);
          }

          // Na parcialmente associativa o intervalo é definido onde a palavra sera inserida de acordo com o bloco 
          blocoMemoria = palavra/inputMemoria.blocos;
          sizeWay = inputCache.linhas/inputMemoria.sizeconjuntos;
          blocoWay = blocoMemoria%inputMemoria.sizeconjuntos;
          inicio = (blocoWay*sizeWay);
          fim = inicio + sizeWay;

           
          if (instrucao.compare("read") == 0)
          {
            //Procurar dentro da cache. 
            estado = findpalavra (inputCache, palavra, blocoCache, inicio, fim);
            if(estado == true)
            {
              std::cout << "HIT linha " << blocoCache << std::endl;
              inputCache.cont[blocoCache] = inputCache.cont[blocoCache] + 1;
              estado = false;
            }
            else
            {
              //Espaço vazio 
              estado = findpalavra (inputCache, -1, blocoCache, inicio, fim);
              if(estado == true)
              {
                mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                inputCache.cont[blocoCache] = 1;
              }
              else
              {
                //Subst. a palavra de acordo com o modo de substituicao 
                blocoCache = inputCache.findMenosUtilizadoscomFrequenciaConjuntos(inputCache, inicio, fim);
                mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                inputCache.cont[blocoCache] = 1;
              }
              std::cout << "MISS -> alocado na linha " << blocoCache << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
            }
          }
          //Show 
          else if (instrucao.compare("show") == 0)
          {
            inputCache.imprimirCache(inputCache, inputMemoria);
            inputMemoria.imprimirMemoria(inputMemoria);
          }
        }
        break;
      // LRU. 
      case 4:
         
        for(int i = 0; i < inputCache.linhas; i++) inputCache.cont.push_back(0);
        while (std::getline (std::cin, instrucao) && instrucao != "exit") 
        {
          found = instrucao.find(" ");
          //Divide  
          if (found!=std::string::npos)
          {
            palavra = std::stoi(instrucao.substr(found + 1));
            instrucao = instrucao.substr(0,found);
          }

          // Na parcialmente associativa o intervalo é definido onde a palavra sera inserida de acordo com o bloco 
          blocoMemoria = palavra/inputMemoria.blocos;
          sizeWay = inputCache.linhas/inputMemoria.sizeconjuntos;
          blocoWay = blocoMemoria%inputMemoria.sizeconjuntos;
          inicio = (blocoWay*sizeWay);
          fim = inicio + sizeWay;
          // read
          if (instrucao.compare("read") == 0)
          {
            // Procurar dentro da cache. 
            estado = findpalavra (inputCache, palavra, blocoCache, inicio, fim);
            if(estado == true)
            {
              std::cout << "HIT linha " << blocoCache << std::endl;
              inputCache.cont[blocoCache] = -1;
              estado = false;
            }
            else
            {
              //Procura espaço na cache 
              estado = findpalavra (inputCache, -1, blocoCache, inicio, fim);
              if(estado == true)
              {
                mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                inputCache.cont[blocoCache] = -1;
              }
              else
              {
                //Subst. a palavra de acordo com o modo de substituicao 
                blocoCache = inputCache.findConjuntosMenosUsadosRecente(inputCache, inicio, fim);
                mudarblocos (inputMemoria, inputCache, blocoCache, blocoMemoria);
                inputCache.cont[blocoCache] = -1;
              }
              std::cout << "MISS -> alocado na linha " << blocoCache << "-> bloco " << blocoMemoria <<" substituido" << std::endl;
            }
            inputCache.mudarConjuntosRecenteUsados(inputCache, inicio, fim);
          }

          //Show 
          else if (instrucao.compare("show") == 0)
          {
            inputCache.imprimirCache(inputCache, inputMemoria);
            inputMemoria.imprimirMemoria(inputMemoria);
          }
        }
        break;
    }
    break;
  }
  inputFile.close();
  return 0;
}