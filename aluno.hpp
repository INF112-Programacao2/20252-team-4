#ifndef ALUNO_HPP
#define ALUNO_HPP

#include <string>
#include <vector>
#include "usuario.hpp"
#include "disciplina.hpp"

class Aluno : public Usuario {
private:
    std::string _nickname;
public:
    Aluno (int ID, int senha, std::string nickname);
    ~Aluno ();
    std::string get_nickname ();
    virtual void avaliar_disciplina (Disciplina disciplina, int turma) override;
    virtual void meu_historico_de_avaliacoes () override;
    virtual void visualizar_avaliacoes (Disciplina disciplina) override;

};

#endif