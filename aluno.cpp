#include "aluno.hpp"
#include "iostream"

//Construtor com parametros
Aluno::Aluno(int id, std::string &nome, std::string &email, std::string &senha) {
    this->id = id;
    this->nome = nome;
    this->email = email;
    this->senha = senha;
}
