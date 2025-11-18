#include "coordenadorCurso.hpp"   
        
CoordCurso::CoordCurso(int id, const std::string &nome, const std::string &email, const std::string &senha){
    this->id = id;
    this->nome = nome;
    this->email = email;
    this->senha = senha;
    this->tipo = "COORDENADOR_DO_CURSO";
}