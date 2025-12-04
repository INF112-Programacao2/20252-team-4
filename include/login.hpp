#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <string>
#include <vector>
#include "usuario.hpp"

//sistema de autenticacao antes de comecar o programa
//pesquisa os usuarios pelo email e valida a senha
class Login {
public:
    //se der certo o ponteiro para o Usuario e' encontrado
    //se der errado, nullptr e lanca excecao
    Usuario * login(const std::vector<Usuario*>& usuarios, const std::string &email, const std::string &senha);
};

#endif