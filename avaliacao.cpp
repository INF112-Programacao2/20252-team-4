#include "avaliacao.hpp"
        
Avaliacao::Avaliacao(int id, int alvoId, std::string &tipo, int nota, std::string &comentario, std::string &data){
    this-> _id = id;
    this-> _alvoId = alvoId;
    this-> _tipo = tipo;
    this-> _nota = nota;
    this-> _comentario = comentario;
    this-> _data = data;
}

int Avaliacao::getId() { 
    return _id; 
}

int Avaliacao::getAlvoId() { 
    return _alvoId; 
}

std::string Avaliacao::getTipo() { 
    return _tipo; 
}

int Avaliacao::getNota() { 
    return _nota; 
}

std::string Avaliacao::getComentario() { 
    return _comentario; 
}

std::string Avaliacao::getData() {
    return _data; 
}