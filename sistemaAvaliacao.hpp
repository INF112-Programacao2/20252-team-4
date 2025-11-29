#ifndef SISTEMAAVALIACAO_HPP
#define SISTEMAAVALIACAO_HPP

#include <vector>
#include <string>

#include "usuario.hpp"
#include "aluno.hpp"
#include "professor.hpp"
#include "coordenadorDisciplina.hpp"
#include "coordenadorCurso.hpp"
#include "disciplina.hpp"
#include "turma.hpp"
#include "avaliacao.hpp"
#include "gerenciadorDados.hpp"

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

public:
    SistemaAvaliacao();
    ~SistemaAvaliacao();

    std::vector<Usuario*>& getUsuarios();

    // Funcoes de cadastro
    void cadastrarUsuario(const std::string &tipo);
    void cadastrarDisciplina();
    void cadastrarTurma();

    // Avaliacao
    void avaliarDisciplina();
    void avaliarProfessor();
    void avaliarTurma();

    // Visualizacao
    void listarAvaliacoes(const std::string &tipo);

    // Persistencia
    void salvarTudo();
};

#endif
