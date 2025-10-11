#ifndef COORDENADORDISCIPLINA_HPP
#define COORDENADORDISCIPLINA_HPP

#include "professor.hpp"

class CoordenadorDisciplina : public Professor {
private:
    Disciplina disciplina;
public:
    CoordenadorDisciplina (int ID, int senha, std::string nome, Disciplina disciplina); //REVER!!!!!!!!!!!!
    Disciplina get_disciplina ();
    virtual void visualizar_avaliacoes (Disciplina disciplina) override;
    void escrever_feedback_das_avaliacoes ();

    
};



#endif