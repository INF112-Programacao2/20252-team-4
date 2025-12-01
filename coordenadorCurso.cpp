#include "coordenadorCurso.hpp"  

CoordenadorCurso::CoordenadorCurso()=default;
        
CoordenadorCurso::CoordenadorCurso(int id, const std::string &nome, const std::string &matricula, const std::string &senha)
    : Professor(id, nome, matricula, senha)  {_tipo = "COORDENADOR_DO_CURSO";}

CoordenadorCurso::CoordenadorCurso(int id, const std::string& nome, const std::string& matricula, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Professor(id, nome, matricula, hashPronto, tipo, hashJaGerado) {}