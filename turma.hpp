#ifndef TURMA_HPP
#define TURMA_HPP

#include "aluno.hpp"
#include "professor.hpp"
#include <vector>

class Turma {
private:
    int _ID;
    std::vector<Aluno> _alunos;
    Professor _professor;

public:
    Turma (int ID, Professor& professor);
    Turma (int ID, Aluno& alunos); //verificar depois
    ~Turma ();
    int get_ID ();
    Aluno get_alunos (); //verificar depois
    Professor get_professor ();
};


#endif