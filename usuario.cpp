#include <iostream>

#include "usuario.hpp"

    Usuario(int id, const std::string& nome, const std::string& email,
            const std::string& senhaClaro, const std::string& tipo)
        : _id(id), _nome(nome), _email(email), _tipo(tipo)
    {
        // transforma a senha pura em SHA-256
        senha_hash = picosha2::hash256_hex_string(senhaClaro);
    }
    ~Usuario() {}
    //get
    int Usuario::getId() const { return id; }
    std::string Usuario::getNome() const { return nome; }
    std::string Usuario::getEmail() const { return email; }
    std::string Usuario::getTipo() const { return tipo; }

    
    // Sera usada pelo gerenciadorDados para gravar no TXT
    std::string getHash() const { return senha_hash; }

    // Autenticacao no login
    bool autenticar(const std::string& senhaDigitada) const {
        return senha_hash == picosha2::hash256_hex_string(senhaDigitada);
    }