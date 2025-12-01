#ifndef COORDCURSO_HPP
#define COORDCURSO_HPP

#include "professor.hpp"

// Coordenador de curso e' o administrador do sistema
// Cadastra usuarios e disciplinas
class CoordenadorCurso : public Professor {
public:
    CoordenadorCurso();
    CoordenadorCurso(int id, const std::string &nome, const std::string &email, const std::string &senha);
    CoordenadorCurso(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado);
};

#endif
