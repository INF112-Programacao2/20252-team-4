#include "coordenadorCurso.hpp"  

//construtor padrao
CoordenadorCurso::CoordenadorCurso()=default;
        
//construtor com parametros
CoordenadorCurso::CoordenadorCurso(int id, const std::string &nome, const std::string &email, const std::string &senha)
    : Professor(id, nome, email, senha)  {_tipo = "COORDENADOR_DO_CURSO";}

//construtor com parametros e hash ja gerado
CoordenadorCurso::CoordenadorCurso(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo, bool hashJaGerado)
    : Professor(id, nome, email, hashPronto, tipo, hashJaGerado) {}