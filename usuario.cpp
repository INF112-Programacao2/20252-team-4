#include <iostream>

#include "usuario.hpp"

   Usuario::Usuario(int id, const std::string& nome, const std::string& matricula,
            const std::string& senhaClaro, const std::string& tipo)
        : _id(id), _nome(nome), _matricula(matricula), _tipo(tipo)
    {
        // transforma a senha pura em SHA-256
        senha_hash = picosha2::hash256_hex_string(senhaClaro);
    }

    Usuario::Usuario(int id, const std::string& nome, const std::string& matricula,
        const std::string& hashPronto, const std::string& tipo,
        bool hashJaGerado) : _id(id), _nome(nome), _matricula(matricula), senha_hash(hashPronto), _tipo(tipo) {}

    Usuario::~Usuario() {}
    //get
    int Usuario::getId() const { return _id; }
    std::string Usuario::getNome() const { return _nome; }
    std::string Usuario::getmatricula() const { return _matricula; }
    std::string Usuario::getTipo() const { return _tipo; }
    std::vector<Turma*> Usuario::getTurmasMatriculadas() const{return _turmasMatriculadas;}

    
    // Sera usada pelo gerenciadorDados para gravar no TXT
    std::string Usuario::getHash() const { return senha_hash; }

    // Autenticacao no login
    bool Usuario::autenticar(const std::string& senhaDigitada){
        return senha_hash == picosha2::hash256_hex_string(senhaDigitada);
    }