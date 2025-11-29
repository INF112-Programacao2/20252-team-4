#ifndef COORDCURSO_HPP
#define COORDCURSO_HPP

#include "usuario.hpp"

// Coordenador de curso e' o administrador do sistema
// Cadastra usuarios e disciplinas
class CoordenadorCurso : public Usuario {
public:
    CoordenadorCurso() = default;
    CoordenadorCurso(int id, const std::string &nome, const std::string &email, const std::string &senha);
};

#endif
