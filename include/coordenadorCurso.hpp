#ifndef COORDCURSO_HPP
#define COORDCURSO_HPP

#include "professor.hpp"

//coordenador de curso e' o administrador do sistema
//ele cadastra usuarios, disciplinas e turmas
class CoordenadorCurso : public Professor {
public:
    CoordenadorCurso();
    //cadastro
    CoordenadorCurso(int id, const std::string &nome, const std::string &email, const std::string &senha);
    //cadastro com hash pronto
    CoordenadorCurso(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado);
};

#endif
