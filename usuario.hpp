#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

// Classe base para todos os usuários do sistema.
// Utiliza herança para permitir tipos de usuários distintos.
class Usuario {
protected:
    int id;
    std::string nome;
    std::string email;
    std::string senha;   // Não criptografada no trabalho.
    std::string tipo;    // "ALUNO", "PROFESSOR", "COORD_DISC", "COORD_CURSO"

public:
    Usuario() = default;
    Usuario(int id, const std::string &_nome,
            const std::string &_email,
            const std::string &_senha,
            const std::string &_tipo):{}

    virtual ~Usuario() {}

    int getId() const {}
    std::string getNome() const {}
    std::string getEmail() const {}
    std::string getTipo() const {}

    // Verifica senha para login
    bool autenticar(const std::string &entrada) const {}
};

#endif