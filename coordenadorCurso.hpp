#ifndef COORDCURSO_HPP
#define COORDCURSO_HPP

#include "Usuario.hpp"

// Coordenador de curso é o administrador do sistema.
// Cadastra usuários e disciplinas.
class CoordCurso : public Usuario {
public:
    CoordCurso() = default;
    CoordCurso(int id, const std::string &nome,
               const std::string &email,
               const std::string &senha)
        : Usuario(id, nome, email, senha, "COORD_CURSO") {}
};

#endif
