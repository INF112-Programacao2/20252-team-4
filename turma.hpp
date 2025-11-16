#ifndef TURMA_HPP
#define TURMA_HPP

#include <string>

class Turma {
private:
    int id;
    int disciplinaId;
    std::string codigoTurma;
    int professorId;

public:
    Turma() = default;

    Turma(int id, int disciplinaId,
          const std::string &codigoTurma,
          int professorId)
        : id(id), disciplinaId(disciplinaId),
          codigoTurma(codigoTurma), professorId(professorId) {}

    int getId() const { return id; }
    int getDisciplinaId() const { return disciplinaId; }
    std::string getCodigoTurma() const { return codigoTurma; }
    int getProfessorId() const { return professorId; }
};

#endif
