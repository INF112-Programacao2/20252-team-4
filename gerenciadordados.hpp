#ifndef GERENCIADORDADOS_HPP
#define GERENCIADORDADOS_HPP

#include <vector>
#include <string>
#include <fstream>
#include "avaliacao.hpp" 
#include "usuario.hpp" 
#include "disciplina.hpp" 
#include "turma.hpp" 

class GerenciadorDados {
private:
    std::vector<Avaliacao> _avaliacoes; 
    std::vector<Usuario> _usuarios; 
    std::vector<Disciplina> _disciplinas; 
    std::vector<Turma> _turmas; 

    const std::string ARQ_AVALIACOES = "avaliacoes.txt"; 
    const std::string ARQ_USUARIOS = "usuarios.txt"; 
    const std::string ARQ_DISCIPLINAS = "disciplinas.txt"; 
    const std::string ARQ_TURMAS = "turmas.txt"; 

public:
    GerenciadorDados(); 
    ~GerenciadorDados(); 
    
    void carregar_avaliacoes(); 
    void carregar_usuarios(); 
    void carregar_disciplinas(); 
    void carregar_turmas(); 

    void salvar_avaliacoes() const; 
    void salvar_usuarios() const; 
    void salvar_disciplinas() const; 
    void salvar_turmas() const; 
    
    void adicionar_avaliacao(const Avaliacao& avaliacao);
    const std::vector<Avaliacao>& get_avaliacoes() const;

    void adicionar_usuario(const Usuario& usuario);
    const std::vector<Usuario>& get_usuarios() const;

    void adicionar_disciplina(const Disciplina& disciplina);
    const std::vector<Disciplina>& get_disciplinas() const;

    void adicionar_turma(const Turma& turma);
    const std::vector<Turma>& get_turmas() const;

    Usuario* buscar_usuario_por_ID(int ID); 
};

#endif