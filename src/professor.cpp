#include "professor.hpp"

//construtor padrao da classe Professor
Professor::Professor() = default;
//construtor da classe Professor
Professor::Professor(int id, const std::string &nome, const std::string &email, const std::string &senha)
    : Usuario(id, nome, email, senha, "PROFESSOR") {}
//construtor da classe Professor com hash ja gerado
Professor::Professor(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Usuario(id, nome, email, hashPronto, tipo, hashJaGerado) {}