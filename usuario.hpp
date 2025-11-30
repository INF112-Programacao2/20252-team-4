#ifndef USUARIO_HPP
#define USUARIO_HPP

#include<vector>
#include <string>
#include "picosha2.hpp"


//Classe base para todos os usuários (aluno, professor, coordenador ..)
//Ter a herança para permitir diferentes tipos de usuários.
class Usuario {
protected:
    int _id;
    std::string _nome;
    std::string _matricula;
    std::string senha_hash;
    std::string _tipo;   // "ALUNO", "PROFESSOR", "COORDENADOR_DISCIPLINA", "COORDENADOR_DO_CURSO"
    std::vector<Turma*> _turmasMatriculadas;


public:
    Usuario() = default;

    Usuario(int id, const std::string &_nome, const std::string &_matricula,
         const std::string &_senha, const std::string &_tipo);

    // Construtor usado APENAS quando carregamos do TXT
    Usuario(int id, const std::string& nome, const std::string& matricula,
        const std::string& hashPronto, const std::string& tipo,
        bool hashJaGerado);

    virtual ~Usuario();

    int getId() const;
    std::string getNome() const;
    std::string getmatricula() const;
    std::string getTipo() const;
    std::vector<Turma*> getTurmasMatriculadas() const;

    // Sera usada pelo gerenciadorDados para gravar no TXT
    std::string getHash() const ;

    //Autenticar login
     bool autenticar(const std::string& senhaDigitada);
};

#endif