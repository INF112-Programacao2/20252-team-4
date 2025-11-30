#include "aluno.hpp"
#include "iostream"


Aluno::Aluno() = default;

//Construtor com parametros
Aluno::Aluno(int id, const std::string &nome, const std::string &matricula, const std::string &senha) : Usuario(id, nome, matricula, senha, "ALUNO")  {}
Aluno::Aluno(int id, const std::string& nome, const std::string& matricula, const std::string& hashPronto, const std::string& tipo,bool hashJaGerado)
: Usuario(id, nome, matricula, hashPronto, tipo, hashJaGerado) {}


