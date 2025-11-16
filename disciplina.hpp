#ifndef DISCIPLINA_HPP
#define DISCIPLINA_HPP

#include <string>

// Objeto básico da avaliação.
// Referencia professor e coordenador.
class Disciplina {
private:
    int id;
    std::string codigo;
    std::string nome;
    int professorId;
    int coordDiscId;

public:
    Disciplina() = default;

    Disciplina(int id, const std::string &codigo,
               const std::string &nome,
               int professorId, int coordDiscId)
        : id(id), codigo(codigo), nome(nome),
          professorId(professorId), coordDiscId(coordDiscId) {}

    int getId() const { return id; }
    std::string getCodigo() const { return codigo; }
    std::string getNome() const { return nome; }
    int getProfessorId() const { return professorId; }
    int getCoordDiscId() const { return coordDiscId; }
};

#endif
