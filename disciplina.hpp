#ifndef DISCIPLINA_HPP
#define DISCIPLINA_HPP

#include <string>
#include <vector>
#include "turma.hpp"

class Disciplina {
private:
    std::string _nome;
    int _codigo;
    std::vector<Turma> _turmas;
    
};

#endif