#ifndef ALUNO_HPP
#define ALUNO_HPP

#include "usuario.hpp"
#include <string>

// Representa um aluno no sistema
// Nao tem permissoes administrativas
class Aluno : public Usuario { //herda da classe usuario e so' tem direito a login

  public:
    Aluno() = default;
    Aluno(int id, const std::string &nome, const std::string &email, const std::string &senha);
};

#endif
