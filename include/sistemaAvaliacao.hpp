#ifndef SISTEMAAVALIACAO_HPP
#define SISTEMAAVALIACAO_HPP

#include <iostream>
#include <algorithm> // para std::find_if e std::sort
#include <limits>   // para std::numeric_limits, serve para limpar o buffer do cin
#include <string>   
#include <map>    // para std::map, serve para armazenar pares chave-valor
#include <numeric> // para std::accumulate, serve para somar elementos de um vetor
#include <iomanip>
#include <vector>

#include "usuario.hpp"
#include "aluno.hpp"
#include "professor.hpp"
#include "coordenadorDisciplina.hpp"
#include "coordenadorCurso.hpp"
#include "disciplina.hpp"
#include "turma.hpp"
#include "avaliacao.hpp"
#include "gerenciadorDados.hpp"
#include "getDataAtual.hpp"

// Essa classe e' o nucleo do sistema
// ela controla:
//   - criacao de usuarios (apenas coordenador do curso)
//   - criacao de disciplinas e turmas
//   - registro de avaliacoes
//   - exibicao das avaliacoes

class SistemaAvaliacao {
private:

    GerenciadorDados arquivo; // cria o gerenciador de dados para persistencia

    // Dados carregados na memoria
    std::vector<Usuario*> _usuarios;
    std::vector<Disciplina> _disciplinas;
    std::vector<Turma> _turmas;
    std::vector<Avaliacao> _avaliacoes;

 // funcoes auxiliares para buscar objetos por ID
    Disciplina* getDisciplinaPorID(int id);
    Turma* getTurmaPorID(int id);
    Usuario* getUsuarioPorID(int id);

public:
// construtor e destrutor
    SistemaAvaliacao();
    ~SistemaAvaliacao();

    //funcoes para obter o proximo ID disponivel
    int ProximoIdUsuarios(const std::vector<Usuario*> &v);
    int ProximoIdDisciplinas(const std::vector<Disciplina> &v);
    int ProximoIdTurmas(const std::vector<Turma> &v);
    int ProximoIdAvaliacoes(const std::vector<Avaliacao> &v);

    // get
    std::vector<Usuario*>& getUsuarios();

    //funcoes de cadastro
    void cadastrarUsuario(const int &tipo);
    void cadastrarDisciplina();
    void cadastrarTurma();
    void matricularAluno();

    //avaliacao
    //para cada tipo de avaliacao
    void avaliarDisciplina(Usuario* u);
    void avaliarProfessor(Usuario *u);
    void avaliarTurma(Usuario* u);

    //visualizacao
    //para cada tipo de visualizacao
    void listarAvaliacoes(const std::string &tipo);
    void visualizarAvaliacoesProfessor(Usuario* u); 
    void visualizarMediasAluno(Usuario* u);     
    void relatorioGeralCoordenador();            

    //persistencia, salvar todos os dados
    void salvarTudo();


    
};

#endif