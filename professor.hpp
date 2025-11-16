#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include "Usuario.hpp"

// O professor pode avaliar turmas e visualizar avaliações.
class Professor : public Usuario {
public:
    Professor() = default;
    Professor(int id, const std::string &nome,
              const std::string &email,
              const std::string &senha)
        : Usuario(id, nome, email, senha, "PROFESSOR") {}
};

#endif
