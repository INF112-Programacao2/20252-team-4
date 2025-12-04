#ifndef ALUNO_HPP
#define ALUNO_HPP

#include "usuario.hpp"
#include <string>

// herda de Usuario pra reaproveitar tudo que é comum: id, nome, email, senha/hash e qualquer método padrão
// nao tem permissoes administrativas
class Aluno : public Usuario { //herda da classe usuario e so' tem direito a login
  public:

    // construtor default para criar o objeto vazio e preencher depois para os carregamentos de arquivos
    Aluno();
    Aluno(int id, const std::string &nome, const std::string &email, const std::string &senha);
    // o construtor normal recebe senha como texto, porém a senha e' hashada antes de salvar
    Aluno(int id, const std::string& nome, const std::string& email, const std::string& hashPronto, const std::string& tipo,bool hashJaGerado);
};

#endif
