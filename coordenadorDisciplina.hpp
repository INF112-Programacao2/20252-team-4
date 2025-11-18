#ifndef COORDDISCIPLINA_HPP
#define COORDDISCIPLINA_HPP

#include "usuario.hpp"

// Coordenador de disciplina.
// Pode ver avaliacoes da disciplina especifica.
class CoordenadorDisciplina : public Usuario {
public:
    CoordenadorDisciplina() = default;
    CoordenadorDisciplina(int id, const std::string &nome, const std::string &email, const std::string &senha) {}
};

#endif
