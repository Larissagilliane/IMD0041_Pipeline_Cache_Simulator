#ifndef __CACHE_H__
#define __CACHE_H__

template <typename T> 
class Memoria //Memoria principal
{
  public:
    T blocos; //Tamanho do bloco em palavras
    T sizeMemoria; //Quantidade de blocos na memoria principal
    T mapeamento; //Mapeamentos 1-Direto 2-Totalmente Associativo 3-Parcialmente Associativo
    T sizeconjuntos; //Numero de conjuntos (parcialmente associativo)
    T politicaSubs; //Politica de subs 1 – Aleatório; 2 – FIFO; 3 – LFU; 4 – LRU
    std::vector<std::vector<T>> elementos;
    
    void criarMemoria (Memoria <T> &inputMemoria) //Criação da memoria principal dado o arquivo de entrada
    {
      std::vector <T> auxpalavras; //Aux
      for(int i = 0; i < (inputMemoria.blocos*inputMemoria.sizeMemoria); i++)
      {
        for(int j = 0; j < inputMemoria.blocos; j++, i++)
        {
          auxpalavras.push_back(i);
        } 
        inputMemoria.elementos.push_back(auxpalavras);
        auxpalavras.clear();
        i--;
      }
    }

    void imprimirMemoria (Memoria <T> &inputMemoria) //Imprime a memória principal
    {
      std::cout << "Memoria Principal" << std::endl;
      std::cout << "Bloco" << " - " << "Endereço" << " - " << "Conteúdo" << std::endl;
      for(size_t i = 0; i < inputMemoria.elementos.size(); i++)
      {
        for(int j = 0; j < inputMemoria.blocos; j++)
        {
          std::cout << i << " - " << inputMemoria.elementos[i][j] << " - " << "-1" <<std::endl;
        }
      }
      std::cout << std::endl;
    }
};


template <typename T> 
class Cache : public Memoria <T> // Classe da memoria cache
{
  public:
    T linhas; //Linhas da cache (que caberão blocos da principal)
    std::vector<std::vector<T>> elementos;
    std::vector <T> cont;
    
    void criarCache (Cache <T> &inputCache) //Cria a memoria cache de acordo com os dados do arquivo
    {
      std::vector <T> auxpalavras;
      for(int i = 0; i < (inputCache.blocos*inputCache.linhas); i++)
      {
        for(int j = 0; j < inputCache.blocos; j++, i++)
        {
          auxpalavras.push_back(-1);
        } 
        inputCache.elementos.push_back(auxpalavras);
        auxpalavras.clear();
        i--;
      }
    }
    
    void imprimirCache (Cache <T> &inputCache, Memoria <T> &inputMemoria) //Imprime a cache
    {
      std::cout << "Cache L1" << std::endl;
      std::cout << "Linha" << " - " << "Bloco" << " - " << "Endereço" << " - " << "Conteudo" << std::endl;
      for(size_t i = 0; i < inputCache.elementos.size(); i++)
      {
        for(int j = 0; j < inputCache.blocos; j++)
        {
          std::cout << i << " - " << inputCache.elementos[i][j]/inputMemoria.blocos << " - " << inputCache.elementos[i][j] << " - " << "-1" <<std::endl;
        }
      }
      std::cout << std::endl;
    }
    
    T findMenosUtilizadoscomFrequenciaConjuntos (Cache <T> &inputCache, T inicio, T fim) //LFU e parcialmente associativo
    {
      T ultimo = inputCache.cont[inicio];
      T ultimoIndice = inicio;
      for(int i = inicio + 1; i < fim; i++) 
      {
        if(inputCache.cont[i] < ultimo)
        {
          ultimo = inputCache.cont[i];
          ultimoIndice = i;
        }
      }
      return ultimoIndice;
    }

    T findMenosUtilizadoscomFrequencia (Cache <T> &inputCache)//LFU e totalmente associativo
    {
      T ultimo = inputCache.cont[0];
      T ultimoIndice = 0;
      for(int i = 1; i < inputCache.linhas; i++)
      {
        if(inputCache.cont[i] < ultimo)
        {
          ultimo = inputCache.cont[i];
          ultimoIndice = i;
        }
      }
      return ultimoIndice;
    }

    T findMenosRecenteUsado(Cache <T> &inputCache)//LRU e totalmente associativo
    {
      T maior = inputCache.cont[0];
      T maiorIndice = 0;
      for(int i = 1; i < inputCache.linhas; i++)
      {
        if(inputCache.cont[i] > maior)
        {
          maior = inputCache.cont[i];
          maiorIndice = i;
        }
      }
      return maiorIndice;
    }

   
    T findConjuntosMenosUsadosRecente(Cache <T> &inputCache, T inicio, T fim)//LRU e parcialmente associativo
    {
      T maior = inputCache.cont[inicio];
      T maiorIndice = inicio;
      for(int i = inicio + 1; i < fim; i++)
      {
        if(inputCache.cont[i] > maior)
        {
          maior = inputCache.cont[i];
          maiorIndice = i;
        }
      }
      return maiorIndice;
    }

    void mudarMenosRecenteUsado(Cache <T> &inputCache)//LRU e totalmente associativo
    {
      for(int i = 0; i < inputCache.linhas; i++)
      {
        inputCache.cont[i] = inputCache.cont[i] + 1;
      }
    }

    void mudarConjuntosRecenteUsados(Cache <T> &inputCache, T inicio, T fim)//LRU e parcialmente associativo
    {
      for(int i = inicio + 1; i < fim; i++)
      {
        inputCache.cont[i] = inputCache.cont[i] + 1;
      }
    }
};

template <typename T> 
void mudarblocos (Memoria <T> &inputMemoria, Cache <T> &inputCache, T blocoCache, T blocoMemoria) //Troca de blocos cache e memoria pricipal
{
  for(T i = 0; i < inputMemoria.blocos; i++)
  {
    inputCache.elementos[blocoCache][i] = inputMemoria.elementos[blocoMemoria][i];
  }
}

template <typename T> 
bool findpalavra (Cache <T> &inputCache, T palavra, T &bloco, T inicio, T fim) //Procurar de endereços dentro da cache
{
  for(int i = inicio; i < fim; i++)
  {
    for(size_t j = 0; j < inputCache.elementos[i].size(); j++)
    {
      if(inputCache.elementos[i][j] == palavra)
      { 
        bloco = i; 
        return true;
      }
    }
  }
  return false;
}
#endif /* __CACHE_H__ */
