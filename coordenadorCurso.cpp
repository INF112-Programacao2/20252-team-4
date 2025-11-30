#include "coordenadorCurso.hpp"  

CoordenadorCurso::CoordenadorCurso()=default;
        
CoordenadorCurso::CoordenadorCurso(int id, const std::string &nome, const std::string &matricula, const std::string &senha)
    : Usuario(id, nome, matricula, senha, "COORDENADOR_DO_CURSO")  {}

CoordenadorCurso::CoordenadorCurso(int id, const std::string& nome, const std::string& matricula, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Usuario(id, nome, matricula, hashPronto, tipo, hashJaGerado) {}