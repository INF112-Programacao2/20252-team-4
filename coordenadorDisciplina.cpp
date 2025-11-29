#include "coordenadorDisciplina.hpp"

CoordenadorDisciplina::CoordenadorDisciplina() = default;

CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& senha) :
    Usuario(id, nome, email, senha, "COORDENADOR_DISCIPLINA") {}

CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Usuario(id, nome, email, hashPronto, tipo, hashJaGerado) {}