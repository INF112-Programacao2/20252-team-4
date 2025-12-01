#include <iostream>

#include "usuario.hpp"

   Usuario::Usuario(int id, const std::string& nome, const std::string& email,
            const std::string& senhaClaro, const std::string& tipo)
        : _id(id), _nome(nome), _email(email), _tipo(tipo)
    {
        // transforma a senha pura em SHA-256
        senha_hash = picosha2::hash256_hex_string(senhaClaro);
    }

    Usuario::Usuario(int id, const std::string& nome, const std::string& email,
        const std::string& hashPronto, const std::string& tipo,
        bool hashJaGerado) : _id(id), _nome(nome), _email(email), senha_hash(hashPronto), _tipo(tipo) {}

    Usuario::~Usuario() {}
    //get
    int Usuario::getId() const { return _id; }
    std::string Usuario::getNome() const { return _nome; }
    std::string Usuario::getemail() const { return _email; }
    std::string Usuario::getTipo() const { return _tipo; }
    const std::vector<Turma*> Usuario::getMinhasDisciplinas() const {return _minhasDisciplinas;}

    
    // Sera usada pelo gerenciadorDados para gravar no TXT
    std::string Usuario::getHash() const { return senha_hash; }

    // Autenticacao no login
    bool Usuario::autenticar(const std::string& senhaDigitada){
        return senha_hash == picosha2::hash256_hex_string(senhaDigitada);
    }