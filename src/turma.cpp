#include "turma.hpp"
//construtor padrao da classe Turma
    Turma::Turma() = default;
//construtor da classe Turma
    Turma::Turma(int id, int disciplinaId, const std::string &codigoTurma, int professorId)
        : _id(id), _disciplinaId(disciplinaId), _codigoTurma(codigoTurma), _professorId(professorId){}
//metodos get da classe Turma
    int Turma::getId() const{ return _id; }
    int Turma::getDisciplinaId() const{ return _disciplinaId; }
    std::string Turma::getCodigoTurma() const{ return _codigoTurma; }
    int Turma::getProfessorId() const{ return _professorId; }