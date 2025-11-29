#include "login.hpp"

Usuario* Login::login(const std::vector<Usuario*>& usuarios, const std::string &email, const std::string &senha) {

    for (auto u : usuarios) {

        if (u->getEmail() == email) {
            if (u->autenticar(senha)) {
                return u;
            }
            else 
                return nullptr;
        }
    }
    
    return nullptr;
}