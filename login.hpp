#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <string>
#include <vector>
#include "../models/Usuario.hpp"

// Sistema de autenticação.
// Pesquisa usuários pelo e-mail e valida senha.
class Login {
public:
    Usuario* login(const std::vector<Usuario*>& usuarios,
                   const std::string &email,
                   const std::string &senha);
};

#endif