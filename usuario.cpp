#include <iostream>

#include "usuario.hpp"

Usuario::Usuario(int id, const std::string &nome,
            const std::string &email,
            const std::string &senha,
            const std::string &tipo)
        : id(id), nome(nome), email(email), senha(senha), tipo(tipo){}

 ~Usuario() {}

int Usuario::getId() const { return id; }
    std::string Usuario::getNome() const { return nome; }
    std::string Usuario::getEmail() const { return email; }
    std::string Usuario::getTipo() const { return tipo; }
