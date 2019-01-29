#ifndef __FUNCOES_H__
#define __FUNCOES_H__

void pipeline (std::vector <std::string> &estagioIF, std::vector <std::string> &estagioID, std::vector <std::string> &estagioEX, std::vector <std::string> &estagioMEM, std::vector <std::string> &estagioWB, std::vector <std::string> Instrucoes, int &ciclo);

int dependecia (const int &ciclo, const std::vector <std::string> &dependeciaregistador, const std::string &register2, const std::string &register3, const std::string &registerAux);

#endif /* __PIPELINE_H__ */
