#ifndef SISTEMAAVALIACAO_HPP
#define SISTEMAAVALIACAO_HPP

#include <vector>
#include <string>

#include "usuario.hpp"
#include "../models/Aluno.hpp"
#include "../models/Professor.hpp"
#include "../models/CoordDisciplina.hpp"
#include "../models/CoordCurso.hpp"
#include "../models/Disciplina.hpp"
#include "../models/Turma.hpp"
#include "../models/Avaliacao.hpp"
#include "../storage/FileManager.hpp"

// Esta classe é o núcleo do sistema.
// Controla:
//   - criação de usuários (apenas coordenador do curso)
//   - criação de disciplinas e turmas
//   - registro de avaliações
//   - exibição das avaliações
class SistemaAvaliacao {
private:
    FileManager arquivo;

    // Dados carregados na memória
    std::vector<Usuario*> usuarios;
    std::vector<Disciplina> disciplinas;
    std::vector<Turma> turmas;
    std::vector<Avaliacao> avaliacoes;

public:
    SistemaAvaliacao();

    // Funções de cadastro
    void cadastrarUsuario(const std::string &tipo);
    void cadastrarDisciplina();
    void cadastrarTurma();

    // Avaliação
    void avaliarDisciplina();
    void avaliarProfessor();
    void avaliarTurma();

    // Visualização
    void listarAvaliacoes(const std::string &tipo);

    // Persistência
    void salvarTudo();
};

#endif
