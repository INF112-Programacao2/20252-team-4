#ifndef DISCIPLINA_HPP
#define DISCIPLINA_HPP

#include <string>
#include <vector>
#include "turma.hpp"

// Objeto basico da avaliacao.
// Referencia professor e coordenador.
class Disciplina {
private:
    int _id;
    std::string _codigo;
    std::string _nome;
    int _professorId;
    int _coordenadorDiscId;
    std::vector<Turma*> _turmasDisciplina;
public:
    Disciplina();

    Disciplina(int id, const std::string &codigo, const std::string &nome, int professorId, int coordenadorDiscId);

    int getId() const;
    std::string getCodigo() const;
    std::string getNome() const;
    int getProfessorId() const;
    int getCoordenadorDiscId() const;
    const std::vector<Turma*> getTurmasDisciplina() const;
};

#endif
