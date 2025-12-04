#ifndef TURMA_HPP
#define TURMA_HPP

#include <string>

//cada turma esta associada a uma disciplina e a uma lista de alunos
class Turma {
private:
    int _id;
    int _disciplinaId;
    std::string _codigoTurma;
    int _professorId;

public:
    //construtores com e sem parametros
    Turma();
    Turma(int id, int disciplinaId, const std::string &codigoTurma, int professorId);

    //gets
    int getId() const;
    int getDisciplinaId() const;
    std::string getCodigoTurma() const;
    int getProfessorId() const;
};   

#endif