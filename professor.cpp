#include "professor.hpp"

Professor::Professor() = default;

Professor::Professor(int id, const std::string &nome, const std::string &email, const std::string &senha)
    : Usuario(id, nome, email, senha, "PROFESSOR") {}

Professor::Professor(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Usuario(id, nome, email, hashPronto, tipo, hashJaGerado) {}