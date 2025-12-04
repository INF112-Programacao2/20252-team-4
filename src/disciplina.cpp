#include "disciplina.hpp"
//construtor padrao
Disciplina::Disciplina() = default;

//construtor com parametros
Disciplina::Disciplina(int id, const std::string& codigo, const std::string& nome, int professorId, int coordenadorDiscId) :
        _id(id), _codigo(codigo), _nome(nome), _professorId(professorId), _coordenadorDiscId(coordenadorDiscId) {}

//getters
int Disciplina::getId() const {
    return _id; 
}

std::string Disciplina::getCodigo() const {
    return  _codigo;
}

std::string Disciplina::getNome() const {
    return _nome;
}

int Disciplina::getProfessorId() const {
    return _professorId;
}

int Disciplina::getCoordenadorDiscId() const {
    return _coordenadorDiscId;
}

const std::vector<Turma*> Disciplina::getTurmasDisciplina() const {
    return _turmasDisciplina;
}



