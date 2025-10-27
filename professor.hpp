#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include <string>
#include <vector>
#include "disciplina.hpp"
#include "usuario.hpp"

class Professor : public Usuario {
private:
    std::string _nome;
public:
    Professor (int ID, int senha, std::string nome);
    ~Professor ();
    std::string get_nome ();
    virtual void avaliar_disciplina (Disciplina& disciplina, Turma& turma) override;
    virtual void meu_historico_de_avaliacoes () override;
    virtual void visualizar_avaliacoes (Disciplina& disciplina) override;
    
};

#endif