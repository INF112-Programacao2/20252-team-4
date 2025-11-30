#ifndef COORDDISCIPLINA_HPP
#define COORDDISCIPLINA_HPP

#include "usuario.hpp"
#include "disciplina.hpp"
#include <vector>

// Coordenador de disciplina.
// Pode ver avaliacoes da disciplina especifica.
class CoordenadorDisciplina : public Usuario {
private:
    Disciplina* _disciplinaCoordenada;    
public:
    CoordenadorDisciplina();
    CoordenadorDisciplina(int id, const std::string &nome, const std::string &matricula, const std::string &senha);
    CoordenadorDisciplina(int id, const std::string& nome, const std::string& matricula, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado);

    const Disciplina* getDisciplinaCoordenada() const;

};

#endif
