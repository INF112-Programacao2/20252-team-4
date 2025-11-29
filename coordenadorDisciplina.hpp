#ifndef COORDDISCIPLINA_HPP
#define COORDDISCIPLINA_HPP

#include "usuario.hpp"

// Coordenador de disciplina.
// Pode ver avaliacoes da disciplina especifica.
class CoordenadorDisciplina : public Usuario {
public:
    CoordenadorDisciplina();
    CoordenadorDisciplina(int id, const std::string &nome, const std::string &email, const std::string &senha);
    CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado);
};

#endif
