/*

CODIGO DE JULIA

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

    //Constantes que guardara o nomes dos arquivos lidos e que serao alterados
    const std::string ARQ_AVALIACOES = "avaliacoes.txt"; // guarda todas as avaliacoes
    const std::string ARQ_USUARIOS = "usuarios.txt"; // guarda todos os informacao dos usuarios cadastrados
    const std::string ARQ_DISCIPLINAS = "disciplinas.txt"; //quardas informacoes das diciplinas
    const std::string ARQ_TURMAS = "turmas.txt"; // quarda informacoes da turma cadastrada

public:
    //Construtor
    GerenciadorDados();
    //Destrutor 
    ~GerenciadorDados(); 
    
    //Fuincao de ler os dados dos arquivos .txt no codigo
    void carregar_avaliacoes(); 
    void carregar_usuarios(); 
    void carregar_disciplinas(); 
    void carregar_turmas(); 

    //Funcao para salvar todos os dados cadastrados no codigo pelo usuario no arquivos .txt
    void salvar_avaliacoes() const; 
    void salvar_usuarios() const; 
    void salvar_disciplinas() const; 
    void salvar_turmas() const; 
    
    //Funcoes de adicao dos objeto na lista e acessos ao dados salvos
    void adicionar_avaliacao(const Avaliacao& avaliacao);
    const std::vector<Avaliacao>& get_avaliacoes() const;

    void adicionar_usuario(const Usuario& usuario);
    const std::vector<Usuario>& get_usuarios() const;

    void adicionar_disciplina(const Disciplina& disciplina);
    const std::vector<Disciplina>& get_disciplinas() const;

    void adicionar_turma(const Turma& turma);
    const std::vector<Turma>& get_turmas() const;

    // Funcao de busca do usuario pelo seu ID.
    Usuario* buscar_usuario_por_ID(int ID); 
};

#endif

*/



#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <string>
#include <vector>

#include "../models/Usuario.hpp"
#include "../models/Disciplina.hpp"
#include "../models/Turma.hpp"
#include "../models/Avaliacao.hpp"

// Classe responsável por ler e salvar arquivos .txt do sistema.
// Cada entidade tem seu próprio arquivo:
//   usuarios.txt
//   disciplinas.txt
//   turmas.txt
//   avaliacoes.txt
// O formato é CSV simples.
class FileManager {
public:
    FileManager() {}

    // CARREGAMENTO
    std::vector<Usuario*> carregarUsuarios();
    std::vector<Disciplina> carregarDisciplinas();
    std::vector<Turma> carregarTurmas();
    std::vector<Avaliacao> carregarAvaliacoes();

    // SALVAMENTO
    void salvarUsuarios(const std::vector<Usuario*>& lista);
    void salvarDisciplinas(const std::vector<Disciplina>& lista);
    void salvarTurmas(const std::vector<Turma>& lista);
    void salvarAvaliacoes(const std::vector<Avaliacao>& lista);
};

#endif
