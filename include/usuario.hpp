#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <vector>
#include "picosha2.hpp" // usada pra gerar o hash SHA-256 da senha
#include "turma.hpp"


//classe base para todos os usuários (aluno, professor, coordenador)
//ter a herança para permitir diferentes tipos de usuários
class Usuario {
protected:
    int _id;
    std::string _nome;
    std::string _email;
    std::string senha_hash;         // senha ja codificada em hash
    std::string _tipo;   // "aluno", "professor", "coordCurso", etc.
    std::vector<Turma*> _minhasDisciplinas;



public:
    Usuario() = default;

    //construtor normal
    Usuario(int id, const std::string &_nome, const std::string &_email, const std::string &_senha, const std::string &_tipo);

    //construtor com o hash gerado
    Usuario(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado);

    virtual ~Usuario();

    //gets
    int getId() const;
    std::string getNome() const;
    std::string getemail() const;
    std::string getTipo() const;
    const std::vector<Turma*> getMinhasDisciplinas() const;
    //sera usada pelo gerenciadorDados para gravar no TXT
    std::string getHash() const ;

    // adiciona uma turma na lista pessoal do usuario e verifica para evitar inserir a mesma turma 2 vzs
    void addTurma(Turma* t);
    // altera o tipo do usuario em tempo de execucao para poder promover o professor para coordenador
    void setTipo(const std::string& novoTipo);

    // recebe a senha digitada e compara com senha hash
     bool autenticar(const std::string& senhaDigitada);
};

#endif