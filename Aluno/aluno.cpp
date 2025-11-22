#include "aluno.hpp"
#include "iostream"

//Construtor com parametros
Aluno::Aluno(int id, const std::string &nome, const std::string &email, const std::string &senha) : Usuario(id, nome, email, senha, "ALUNO")  {}

