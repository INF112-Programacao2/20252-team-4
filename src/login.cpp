#include "login.hpp"

//implementacao do metodo de login
Usuario* Login::login(const std::vector<Usuario*>& usuarios, const std::string &email, const std::string &senha) {

    for (auto u : usuarios) {

        if (u->getemail() == email) {
            if (u->autenticar(senha)) {
                return u;
            }
            else 
                return nullptr;
        }
    }
    
    return nullptr;
}