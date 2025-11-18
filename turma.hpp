#ifndef TURMA_HPP
#define TURMA_HPP

#include <string>

class Turma {
private:
    int _id;
    int _disciplinaId;
    std::string _codigoTurma;
    int _professorId;

public:
    Turma() = default;

    Turma(int id, int disciplinaId, const std::string &codigoTurma, int professorId){}

    int getId() const{}
    int getDisciplinaId() const{}
    std::string getCodigoTurma() const{}
    int getProfessorId() const{}
};

#endif
