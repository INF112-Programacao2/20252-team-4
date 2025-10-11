#ifndef COORDENADORCURSO_HPP
#define COORDENADORCURSO_HPP

#include "professor.hpp"

class CoordenadorCurso : public Professor {
public:
    CoordenadorCurso (int ID, int senha, std::string nome); //REVER!!!!!!!!!!!!
    virtual void visualizar_avaliacoes (Disciplina disciplina) override;
    void escrever_feedback_das_avaliacoes ();
    
};



#endif