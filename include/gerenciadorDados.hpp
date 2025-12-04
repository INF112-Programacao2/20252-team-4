#ifndef GERENCIADORDADOS_HPP
#define GERENCIADORDADOS_HPP

#include <string>
#include <vector>

#include "usuario.hpp"
#include "aluno.hpp"
#include "professor.hpp"
#include "coordenadorCurso.hpp"
#include "coordenadorDisciplina.hpp"
#include "disciplina.hpp"
#include "turma.hpp"
#include "avaliacao.hpp"
#include "getDataAtual.hpp"

// Classe responsavel por ler e salvar arquivos .txt do sistema
// Cada entidade tem seu proprio arquivo
//   usuarios.txt
//   disciplinas.txt
//   turmas.txt
//   avaliacoes.txt
class GerenciadorDados {
public:
    //o destrutor e' útil para abrir arquivos, gerenciar recursos, etc
    GerenciadorDados();
    ~GerenciadorDados();

    //normalmente le o "tipo" no arquivo e instancia Aluno/Professor/Coord
    void carregarUsuarios(std::vector<Usuario*>& usuarios);

    //retornam vetores por valor com entidades carregadas
    std::vector<Disciplina> carregarDisciplinas();
    std::vector<Turma> carregarTurmas();
    std::vector<Avaliacao> carregarAvaliacoes();
    // pra preencher as listas de matrícula a partir do arquivo.
    void carregarMatriculas(std::vector<Usuario*>& usuarios, std::vector<Turma>& turmas);

    // SALVAMENTO

    //salva cada entidade no seu arquivo correspondente
    //Usuarios são ponteiros por causa do polimorfismo (Aluno/Professor/Coords)
    void salvarUsuarios(const std::vector<Usuario*>& lista);
    void salvarDisciplinas(const std::vector<Disciplina>& lista);
    void salvarTurmas(const std::vector<Turma>& lista);
    void salvarAvaliacoes(const std::vector<Avaliacao>& lista);
    //salva as matriculas
    void salvarMatriculas(const std::vector<Usuario*>& lista) const;    
};

#endif
