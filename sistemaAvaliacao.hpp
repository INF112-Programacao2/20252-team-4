#ifndef SISTEMAAVALIACAO_HPP
#define SISTEMAAVALIACAO_HPP

#include <iostream>
#include <algorithm>
#include <limits>
#include <string> 
#include <map>    // NEW: Necessário para std::map
#include <numeric>// NEW: Necessário para std::accumulate
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
    GerenciadorDados arquivo;

    // Dados carregados na memoria
    std::vector<Usuario*> _usuarios;
    std::vector<Disciplina> _disciplinas;
    std::vector<Turma> _turmas;
    std::vector<Avaliacao> _avaliacoes;

    Disciplina* getDisciplinaById(int id);
    Turma* getTurmaById(int id);
    Usuario* getUsuarioById(int id);

public:
    SistemaAvaliacao();
    ~SistemaAvaliacao();

    int ProximoIdUsuarios(const std::vector<Usuario*> &v);
    int ProximoIdDisciplinas(const std::vector<Disciplina> &v);
    int ProximoIdTurmas(const std::vector<Turma> &v);
    int ProximoIdAvaliacoes(const std::vector<Avaliacao> &v);

    std::vector<Usuario*>& getUsuarios();

    // Funcoes de cadastro
    void cadastrarUsuario(const int &tipo);
    void cadastrarDisciplina();
    void cadastrarTurma();
    void matricularAluno();

    // Avaliacao
    void avaliarDisciplina(Usuario* u);
    void avaliarProfessor(Usuario *u);
    void avaliarTurma(Usuario* u);

    // Visualizacao
    void listarAvaliacoes(const std::string &tipo);
    void visualizarAvaliacoesProfessor(Usuario* u); // Ponto 1 e 2: Professor e Coordenador de Discplina
    void visualizarMediasAluno(Usuario* u);        // Ponto 3: Aluno
    void relatorioGeralCoordenador();             // Ponto 4: Coordenador

    // Persistencia
    void salvarTudo();


    
};

#endif
