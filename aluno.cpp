#include "aluno.hpp"
#include "iostream"

//Construtor com parametros
Aluno::Aluno(int id, const std::string &nome, const std::string &email, const std::string &senha) {
    this->id = id;
    this->nome = nome;
    this->email = email;
    this->senha = senha;
}
