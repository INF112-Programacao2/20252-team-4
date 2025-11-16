#ifndef COORDDISCIPLINA_HPP
#define COORDDISCIPLINA_HPP

#include "Usuario.hpp"

// Coordenador de disciplina.
// Pode ver avaliações da disciplina específica.
class CoordDisciplina : public Usuario {
public:
    CoordDisciplina() = default;
    CoordDisciplina(int id, const std::string &nome,
                    const std::string &email,
                    const std::string &senha)
        : Usuario(id, nome, email, senha, "COORD_DISC") {}
};

#endif
