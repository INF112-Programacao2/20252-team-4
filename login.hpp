#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <string>
#include <vector>
#include "usuario.hpp"

// Sistema de autenticação.
// Pesquisa usuários pelo e-mail e valida senha.
class Login {
public:
    Usuario * fazerLogin(const std::vector<Usuario*>& usuarios);
};

#endif