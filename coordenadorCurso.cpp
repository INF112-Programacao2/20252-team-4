#include "coordenadorCurso.hpp"  

CoordenadorCurso::CoordenadorCurso()=default;
        
CoordenadorCurso::CoordenadorCurso(int id, const std::string &nome, const std::string &email, const std::string &senha)
    : Professor(id, nome, email, senha)  {_tipo = "COORDENADOR_DO_CURSO";}

CoordenadorCurso::CoordenadorCurso(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Professor(id, nome, email, hashPronto, tipo, hashJaGerado) {}