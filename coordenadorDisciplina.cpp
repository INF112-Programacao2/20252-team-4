#include "coordenadorDisciplina.hpp"

CoordenadorDisciplina::CoordenadorDisciplina() = default;

CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& senha) :
    Professor(id, nome, email, senha) {_tipo = "COORDENADOR_DISCIPLINA";}

CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Professor(id, nome, email, hashPronto, tipo, hashJaGerado) {}

const Disciplina* CoordenadorDisciplina::getDisciplinaCoordenada() const {
    return _disciplinaCoordenada;
}