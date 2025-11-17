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
    int _nota;                // 0 a 10
    std::string _comentario;  // comentario do avaliador
    std::string _data;        // data da avaliacao

public:
    Avaliacao() = default;
    Avaliacao(int id, int alvoId, std::string &tipo, int nota, std::string &comentario, std::string &data);

    int getId();
    int getAlvoId();
    std::string getTipo();
    int getNota();
    std::string getComentario();
    std::string getData();
};

#endif
