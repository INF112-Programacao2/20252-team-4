#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

//Classe base para todos os usuários (aluno, professor, coordenador ..)
//Ter a herança para permitir diferentes tipos de usuários.
class Usuario {
protected:
<<<<<<< HEAD
    int id;
    std::string nome;
    std::string email;
    std::string senha;   // Não criptografada no trabalho.
    std::string tipo;    // "ALUNO", "PROFESSOR", "COORDENADOR_DA_DISCIPLINA", "COORDENADOR_DO_CURSO"
=======
    int _id;
    std::string _nome;
    std::string _email;
    std::string _senha;
    std::string _tipo;   // "ALUNO", "PROFESSOR", "COORD_DISCIPLINA", "COORD_CURSO"
>>>>>>> c767c5b4cc3d829afa566cbebce30e6b461d96f5

public:
    Usuario() = default;

    Usuario(int id, const std::string &_nome, const std::string &_email, const std::string &_senha, const std::string &_tipo){}

    virtual ~Usuario(){}

<<<<<<< HEAD
    // Verifica senha para login
    bool autenticar(const std::string &entrada) const {}
=======
    int getId() const{}
    std::string getNome() const{}
    std::string getEmail() const{}
    std::string getTipo() const{}

    //Autenticar login
    bool autenticar(const std::string &entrada) const{}
>>>>>>> c767c5b4cc3d829afa566cbebce30e6b461d96f5
};

#endif