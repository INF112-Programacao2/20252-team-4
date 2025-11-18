#include <iostream>

#include "usuario.hpp"

Usuario::Usuario() = default;

Usuario::Usuario(int id, const std::string &nome, const std::string &email, const std::string &senha, const std::string &tipo)
        : id(_id), nome(_nome), email(_email), senha(_senha), tipo(_tipo){}

Usuario::~Usuario(){}

int Usuario::getId() const{ return _id; }
std::string Usuario::getNome() const{ return _nome; }
std::string Usuario::getEmail() const{ return _email; }
std::string Usuario::getTipo() const{ return _tipo; }
