#include "professor.hpp"

Professor::Professor() = default;

Professor::Professor(int id, const std::string &nome, const std::string &matricula, const std::string &senha)
    : Usuario(id, nome, matricula, senha, "PROFESSOR") {}

Professor::Professor(int id, const std::string& nome, const std::string& matricula, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Usuario(id, nome, matricula, hashPronto, tipo, hashJaGerado) {}