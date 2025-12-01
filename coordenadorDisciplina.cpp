#include "coordenadorDisciplina.hpp"

CoordenadorDisciplina::CoordenadorDisciplina() = default;

CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& matricula, const std::string& senha) :
    Professor(id, nome, matricula, senha) {_tipo = "COORDENADOR_DISCIPLINA";}

CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& matricula, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Professor(id, nome, matricula, hashPronto, tipo, hashJaGerado) {}

const Disciplina* CoordenadorDisciplina::getDisciplinaCoordenada() const {
    return _disciplinaCoordenada;
}