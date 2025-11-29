#include "aluno.hpp"
#include "iostream"


Aluno::Aluno() = default;

//Construtor com parametros
Aluno::Aluno(int id, const std::string &nome, const std::string &email, const std::string &senha) : Usuario(id, nome, email, senha, "ALUNO")  {}
Aluno::Aluno(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo,bool hashJaGerado)
: Usuario(id, nome, email, hashPronto, tipo, hashJaGerado) {}


