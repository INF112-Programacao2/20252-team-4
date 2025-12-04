#ifndef COORDDISCIPLINA_HPP
#define COORDDISCIPLINA_HPP

#include "professor.hpp"
#include "disciplina.hpp"
#include <vector>

//pode ver avaliacoes do professor e da disciplina especifica
class CoordenadorDisciplina : public Professor {
private:
    // disciplina que esse coordenador coordena
    Disciplina* _disciplinaCoordenada;    
public:
    CoordenadorDisciplina();
    //cadastro normal
    CoordenadorDisciplina(int id, const std::string &nome, const std::string &email, const std::string &senha);
    //cadastro com hash pronto
    CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado);

    //get
    const Disciplina* getDisciplinaCoordenada() const; //retorna const Disciplina* pra impedir alteracao direta pela referencia

};

#endif
