#ifndef UTILIDADES_HPP
#define UTILIDADES_HPP
#include <string>
#include <vector>

class Utilidades {
public:
    std::string getDataAtual();
    std::string lerSenhaMascarada(const std::string& mensagem);
    int lerInteiroComExcecao(const std::string& pergunta);
    std::vector<std::string> dividirLinha(const std::string &linha, char delim=';');
};

#endif