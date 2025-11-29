#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <string>
#include <vector>
#include "usuario.hpp"

// Sistema de autenticação.
// Pesquisa usuários pelo e-mail e valida senha.
class Login {
public:
    Usuario *login(const std::vector<Usuario*>& usuarios, const std::string &email, const std::string &senha);
};

#endif
// --- ALTERAR APENAS A PARTE DO LOGIN ---
/*
bool Sistema::login(const std::string& email, const std::string& senha) {
    for (auto u : usuarios) {
        if (u->getEmail() == email && u->autenticar(senha)) {
            usuarioLogado = u;
            return true;
        }
    }
    return false;
}
*/