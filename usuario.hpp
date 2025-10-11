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
    int get_ID ();
    int get_senha ();
    Disciplina* get_disciplinas ();
    Usuario (int ID, int senha);
    ~Usuario ();
    virtual void avaliar_disciplina (Disciplina disciplina, int turma);
    virtual void meu_historico_de_avaliacoes ();
    virtual void visualizar_avaliacoes (Disciplina disciplina);
    
};

#endif