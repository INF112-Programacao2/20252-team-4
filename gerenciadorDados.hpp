#ifndef GERENCIADORDADOS_HPP
#define GERENCIADORDADOS_HPP

#include <string>
#include <vector>

#include "usuario.hpp"
#include "disciplina.hpp"
#include "turma.hpp"
#include "avaliacao.hpp"

// Classe responsável por ler e salvar arquivos .txt do sistema.
// Cada entidade tem seu próprio arquivo:
//   usuarios.txt
//   disciplinas.txt
//   turmas.txt
//   avaliacoes.txt
// O formato é CSV simples.
class GerenciadorDados {
public:
    GerenciadorDados();

    ~GerenciadorDados();

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
