#ifndef AVALIACAO_HPP
#define AVALIACAO_HPP

#include <string>
#include <fstream>
#include "disciplina.hpp"
#include "turma.hpp"
#include "usuario.hpp"

class Avaliacao {
private:
    int _idAvaliacao;
    std::string _tipo;
    Disciplina _disciplina;
    Turma _turma;
    Usuario _usuario;
    std::string _comentario;

public:
    Avaliacao(Disciplina& disciplina, Turma& turma, Usuario& usuario);
    ~Avaliacao();
    std::string get_tipo() const;
    Disciplina get_disciplina() const;
    Turma get_turma() const;
    Usuario get_usuario() const;
};





#endif