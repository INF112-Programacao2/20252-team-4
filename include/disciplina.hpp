#ifndef DISCIPLINA_HPP
#define DISCIPLINA_HPP

#include <string>
#include <vector>
#include "turma.hpp"

//objeto basico da avaliacao
//referencia professor e coordenador
class Disciplina {
private:
    int _id;                //id da disciplina
    std::string _codigo;    //codigo da disciplina
    std::string _nome;    //nome da disciplina
    int _professorId;        // professor dela
    int _coordenadorDiscId;            //coordenador da disciplina
    std::vector<Turma*> _turmasDisciplina;   //turmas dessa disciplina
public:
    Disciplina();
    //construtor
    Disciplina(int id, const std::string &codigo, const std::string &nome, int professorId, int coordenadorDiscId);

    //getters
    int getId() const;
    std::string getCodigo() const;
    std::string getNome() const;
    int getProfessorId() const;
    int getCoordenadorDiscId() const;
    const std::vector<Turma*> getTurmasDisciplina() const;
};

#endif
