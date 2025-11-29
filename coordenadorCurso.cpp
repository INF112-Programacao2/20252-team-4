#include "coordenadorCurso.hpp"  

CoordenadorCurso::CoordenadorCurso()=default;
        
CoordenadorCurso::CoordenadorCurso(int id, const std::string &nome, const std::string &email, const std::string &senha)
 : Usuario(id, nome, email, senha, "COORDENADOR_DO_CURSO")  {}