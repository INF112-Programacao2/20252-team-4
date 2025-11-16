#ifndef ALUNO_HPP
#define ALUNO_HPP

#include "Usuario.hpp"

// Representa um aluno no sistema.
// Não tem permissões administrativas.
class Aluno : public Usuario {
public:
    Aluno() = default;
    Aluno(int id, const std::string &nome,
          const std::string &email,
          const std::string &senha)
        : Usuario(id, nome, email, senha, "ALUNO") {}
};

#endif
