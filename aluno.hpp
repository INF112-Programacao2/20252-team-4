#ifndef ALUNO_HPP
#define ALUNO_HPP

#include "Usuario.hpp"
#include <string>

// Representa um aluno no sistema
// Nao tem permissões administrativas
class Aluno : public Usuario { //herda da classe usuario e so' tem direito a login

  public:
    Aluno() = default;
    Aluno(int id, std::string &nome, std::string &email, std::string &senha);
};

#endif
