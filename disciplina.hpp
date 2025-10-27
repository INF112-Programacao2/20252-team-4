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

public:
    Disciplina (std::string nome, int codigo, std::vector<Turma> turmas);
    ~Disciplina();
    std::string get_nome () const;
    int getCodigo () const;
    std::vector<Turma> getTurmas() const;
    
};

#endif