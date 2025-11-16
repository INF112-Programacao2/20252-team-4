#ifndef AVALIACAO_HPP
#define AVALIACAO_HPP

#include <string>

// Representa uma avaliação anônima.
// Pode ser destinada a disciplina, professor ou turma.
class Avaliacao {
private:
    int id;
    int alvoId;            // disciplina/professor/turma
    std::string tipo;      // "DISCIPLINA", "PROFESSOR", "TURMA"
    int nota;
    std::string comentario;

public:
    Avaliacao() = default;

    Avaliacao(int id, int alvoId,
              const std::string &tipo,
              int nota,
              const std::string &comentario)
        : id(id), alvoId(alvoId), tipo(tipo),
          nota(nota), comentario(comentario) {}

    int getId() const { return id; }
    int getAlvoId() const { return alvoId; }
    std::string getTipo() const { return tipo; }
    int getNota() const { return nota; }
    std::string getComentario() const { return comentario; }
};

#endif
