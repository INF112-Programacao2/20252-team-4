#include "coordenadorDisciplina.hpp"

CoordenadorDisciplina::CoordenadorDisciplina() = default;

CoordenadorDisciplina::CoordenadorDisciplina(int id, const std::string& nome, const std::string& email, const std::string& senha) :
    Usuario(id, nome, email, senha, "COORDENADOR_DISCIPLINA") {}