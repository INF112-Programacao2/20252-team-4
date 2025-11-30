#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include "usuario.hpp"

// O professor pode avaliar turmas e visualizar avaliacoes.
class Professor : public Usuario {
public:
    Professor();
    Professor(int id, const std::string &nome, const std::string &matricula, const std::string &senha);
    Professor(int id, const std::string& nome, const std::string& matricula, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado);
};
#endif
