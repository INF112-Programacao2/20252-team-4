#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <vector>
#include "disciplina.hpp"

class Usuario {
private:
    int _ID;
    int _senha;
    std::vector<Disciplina> disciplinas;
public:
    int get_ID () const;
    int get_senha () const;
    Disciplina* const get_disciplinas () const;
    Usuario (int ID, int senha);
    ~Usuario ();
    virtual void avaliar_disciplina (Disciplina& disciplina, Turma& turma);
    virtual void meu_historico_de_avaliacoes ();
    virtual void visualizar_avaliacoes (Disciplina& disciplina);
    
};

#endif