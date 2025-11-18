#ifndef COORDENADORDISCIPLINA_HPP
#define COORDENADORDISCIPLINA_HPP

#include "usuario.hpp"

// Pode ver avaliações da disciplina especifica.
class CoordenadorDisciplina : public Usuario {
public:
    CoordenadorDisciplina();
    CoordenadorDisciplina(int id, const std::string &nome, const std::string &email, const std::string &senha);
};

#endif
