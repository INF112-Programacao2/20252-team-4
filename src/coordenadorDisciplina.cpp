#include "coordenadorDisciplina.hpp"

//construtor padrao
CoordenadorDisciplina::CoordenadorDisciplina() = default;

//construtor com parametros
CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& senha) :
    Professor(id, nome, email, senha) {_tipo = "COORDENADOR_DISCIPLINA";}

//construtor com parametros e hash ja gerado
CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Professor(id, nome, email, hashPronto, tipo, hashJaGerado) {}
//get para disciplina coordenada
const Disciplina* CoordenadorDisciplina::getDisciplinaCoordenada() const {
    return _disciplinaCoordenada;
}