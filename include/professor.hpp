#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include "usuario.hpp"

//o professor pode avaliar turmas e visualizar as avaliacoes feitas para ele
class Professor : public Usuario {
public:
    //construtor default para carregamento
    Professor();
    //construtor para cadastro
    Professor(int id, const std::string &nome, const std::string &email, const std::string &senha);
    //construtor para carregamento com o hash ja' pronto
    Professor(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado);
};
#endif
