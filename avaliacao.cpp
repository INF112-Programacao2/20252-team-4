#include "avaliacao.hpp"

Avaliacao::Avaliacao() = default;
        
Avaliacao::Avaliacao(int id, int alvoId, const std::string &tipo, double nota, const std::string &comentario, const std::string &data){
    this-> _id = id;
    this-> _alvoId = alvoId;
    this-> _tipo = tipo;
    this-> _nota = nota;
    this-> _comentario = comentario;
    this-> _data = data;
}

int Avaliacao::getId() const { 
    return _id; 
}

int Avaliacao::getAlvoId() const { 
    return _alvoId; 
}

std::string Avaliacao::getTipo() const { 
    return _tipo; 
}

double Avaliacao::getNota() const { 
    return _nota; 
}

std::string Avaliacao::getComentario() const { 
    return _comentario; 
}

std::string Avaliacao::getData() const {
    return _data; 
}