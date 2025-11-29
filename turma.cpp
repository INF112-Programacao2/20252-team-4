#include "turma.hpp"

    Turma::Turma() = default;

    Turma::Turma(int id, int disciplinaId, const std::string &codigoTurma, int professorId)
        : _id(id), _disciplinaId(disciplinaId), _codigoTurma(codigoTurma), _professorId(professorId){}

    int Turma::getId() const{ return _id; }
    int Turma::getDisciplinaId() const{ return _disciplinaId; }
    std::string Turma::getCodigoTurma() const{ return _codigoTurma; }
    int Turma::getProfessorId() const{ return _professorId; }