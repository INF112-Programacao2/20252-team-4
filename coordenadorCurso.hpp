#ifndef COORDCURSO_HPP
#define COORDCURSO_HPP

#include "usuario.hpp"

// Coordenador de curso e' o administrador do sistema
// Cadastra usuarios e disciplinas
class CoordCurso : public Usuario {
public:
    CoordCurso() = default;
    CoordCurso(int id, const std::string &nome, const std::string &email, const std::string &senha);
};

#endif

