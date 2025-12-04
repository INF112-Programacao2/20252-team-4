#ifndef AVALIACAO_HPP
#define AVALIACAO_HPP

#include <string>

// Representa uma avaliação anonima
// Pode ser destinada a disciplina, professor ou turma

class Avaliacao {

private:
    int _id;                   // id da avaliacao
    int _alvoId;              // id da disciplina/professor/turma
    std::string _tipo;        // disciplina, professor ou turma
    double _nota;                // 0 a 5
    std::string _comentario;  // comentario do avaliador
    std::string _data;        // data da avaliacao

public:
    Avaliacao();
    Avaliacao(int id, int alvoId, const std::string &tipo, double nota, const std::string &comentario, const std::string &data);

    int getId() const;
    int getAlvoId() const;
    std::string getTipo() const;
    double getNota() const;
    std::string getComentario() const;
    std::string getData() const;
};

#endif
