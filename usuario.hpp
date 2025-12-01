#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <vector>
#include "picosha2.hpp"
#include "turma.hpp"


//Classe base para todos os usuários (aluno, professor, coordenador ..)
//Ter a herança para permitir diferentes tipos de usuários.
class Usuario {
protected:
    int _id;
    std::string _nome;
    std::string _email;
    std::string senha_hash;
    std::string _tipo;   // "ALUNO", "PROFESSOR", "COORDENADOR_DISCIPLINA", "COORDENADOR_DO_CURSO"
    std::vector<Turma*> _minhasDisciplinas;



public:
    Usuario() = default;

    Usuario(int id, const std::string &_nome, const std::string &_email,
         const std::string &_senha, const std::string &_tipo);

    // Construtor usado APENAS quando carregamos do TXT
    Usuario(int id, const std::string& nome, const std::string& email,
        const std::string& hashPronto, const std::string& tipo,
        bool hashJaGerado);

    virtual ~Usuario();

    int getId() const;
    std::string getNome() const;
    std::string getemail() const;
    std::string getTipo() const;
    const std::vector<Turma*> getMinhasDisciplinas() const;

    // Sera usada pelo gerenciadorDados para gravar no TXT
    std::string getHash() const ;

    //Autenticar login
     bool autenticar(const std::string& senhaDigitada);
};

#endif