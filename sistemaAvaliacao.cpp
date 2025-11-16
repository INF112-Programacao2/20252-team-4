#include "SistemaAvaliacao.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

// Construtor: carrega tudo do FileManager
SistemaAvaliacao::SistemaAvaliacao() {
    usuarios = arquivo.carregarUsuarios();
    disciplinas = arquivo.carregarDisciplinas();
    turmas = arquivo.carregarTurmas();
    avaliacoes = arquivo.carregarAvaliacoes();

    // Se não existir nenhum coordenador do curso, cria um padrão
    bool temCoord = false;
    for (auto u : usuarios) if (u->getTipo() == "COORD_CURSO") { temCoord = true; break; }
    if (!temCoord) {
        // adiciona um administrador padrão (id = 1)
        CoordCurso* admin = new CoordCurso(1, "Admin Curso", "admin@ufv.br", "admin123");
        usuarios.insert(usuarios.begin(), admin);
    }
}

// destrutor: libera ponteiros de usuarios
SistemaAvaliacao::~SistemaAvaliacao() {
    for (auto u : usuarios) delete u;
}

// ----- auxiliar: gerar novo id para cada tipo -----
static int nextIdForUsuarios(const std::vector<Usuario*>& v) {
    int maxid = 0;
    for (auto u : v) if (u->getId() > maxid) maxid = u->getId();
    return maxid + 1;
}
static int nextIdForDisciplinas(const std::vector<Disciplina>& v) {
    int maxid = 0;
    for (auto &d : v) if (d.getId() > maxid) maxid = d.getId();
    return maxid + 1;
}
static int nextIdForTurmas(const std::vector<Turma>& v) {
    int maxid = 0;
    for (auto &t : v) if (t.getId() > maxid) maxid = t.getId();
    return maxid + 1;
}
static int nextIdForAvaliacoes(const std::vector<Avaliacao>& v) {
    int maxid = 0;
    for (auto &a : v) if (a.getId() > maxid) maxid = a.getId();
    return maxid + 1;
}

// ----- getters (para compatibilidade com main.cpp) -----
std::vector<Usuario*>& SistemaAvaliacao::getUsuarios() {
    return usuarios;
}

// ----- CADASTROS -----
void SistemaAvaliacao::cadastrarUsuario(const std::string &tipo) {
    std::string nome, email, senha;
    std::cout << "Nome: ";
    std::getline(std::cin >> std::ws, nome);
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Senha: ";
    std::cin >> senha;

    int id = nextIdForUsuarios(usuarios);

    Usuario* novo = nullptr;
    if (tipo == "ALUNO") novo = new Aluno(id, nome, email, senha);
    else if (tipo == "PROFESSOR") novo = new Professor(id, nome, email, senha);
    else if (tipo == "COORD_DISC") novo = new CoordDisciplina(id, nome, email, senha);
    else if (tipo == "COORD_CURSO") novo = new CoordCurso(id, nome, email, senha);
    else {
        std::cout << "Tipo inválido. Operação cancelada.\n";
        return;
    }

    usuarios.push_back(novo);
    std::cout << "Usuário cadastrado com ID: " << id << "\n";
}

void SistemaAvaliacao::cadastrarDisciplina() {
    std::string codigo, nome;
    int profId, coordId;
    std::cout << "Código da disciplina (ex: INF112): ";
    std::cin >> codigo;
    std::cout << "Nome da disciplina: ";
    std::getline(std::cin >> std::ws, nome);

    // listar professores disponíveis
    std::cout << "Professores disponíveis:\n";
    for (auto u : usuarios) if (u->getTipo() == "PROFESSOR")
        std::cout << u->getId() << " - " << u->getNome() << '\n';
    std::cout << "Escolha o ID do professor responsável: ";
    std::cin >> profId;

    std::cout << "Coordenadores de disciplina disponíveis:\n";
    for (auto u : usuarios) if (u->getTipo() == "COORD_DISC")
        std::cout << u->getId() << " - " << u->getNome() << '\n';
    std::cout << "Escolha o ID do coordenador da disciplina: ";
    std::cin >> coordId;

    int id = nextIdForDisciplinas(disciplinas);
    disciplinas.emplace_back(id, codigo, nome, profId, coordId);
    std::cout << "Disciplina cadastrada (ID=" << id << ")\n";
}

void SistemaAvaliacao::cadastrarTurma() {
    int disciplinaId;
    std::string codigoTurma;
    int professorId;

    std::cout << "Disciplinas disponíveis:\n";
    for (auto &d : disciplinas) std::cout << d.getId() << " - " << d.getCodigo() << " " << d.getNome() << '\n';
    std::cout << "Escolha ID da disciplina: ";
    std::cin >> disciplinaId;

    std::cout << "Codigo da turma (ex: A): ";
    std::cin >> codigoTurma;

    std::cout << "Professores disponíveis:\n";
    for (auto u : usuarios) if (u->getTipo() == "PROFESSOR")
        std::cout << u->getId() << " - " << u->getNome() << '\n';
    std::cout << "Escolha ID do professor para a turma: ";
    std::cin >> professorId;

    int id = nextIdForTurmas(turmas);
    turmas.emplace_back(id, disciplinaId, codigoTurma, professorId);
    std::cout << "Turma cadastrada (ID=" << id << ")\n";
}

// ----- AVALIACOES -----
void SistemaAvaliacao::avaliarDisciplina() {
    if (disciplinas.empty()) {
        std::cout << "Nenhuma disciplina cadastrada.\n";
        return;
    }
    std::cout << "Disciplinas:\n";
    for (auto &d : disciplinas) std::cout << d.getId() << " - " << d.getCodigo() << " " << d.getNome() << '\n';
    int idDisc;
    std::cout << "Escolha ID da disciplina: ";
    std::cin >> idDisc;
    int nota;
    std::cout << "Nota (0-10): ";
    std::cin >> nota;
    std::string comentario;
    std::cout << "Comentario: ";
    std::getline(std::cin >> std::ws, comentario);

    int id = nextIdForAvaliacoes(avaliacoes);
    avaliacoes.emplace_back(id, idDisc, "DISCIPLINA", nota, comentario);
    std::cout << "Avaliação anônima registrada.\n";
}

void SistemaAvaliacao::avaliarProfessor() {
    std::cout << "Professores:\n";
    for (auto u : usuarios) if (u->getTipo() == "PROFESSOR") std::cout << u->getId() << " - " << u->getNome() << '\n';
    int profId; std::cout << "Escolha ID do professor: "; std::cin >> profId;
    int nota; std::cout << "Nota (0-10): "; std::cin >> nota;
    std::string comentario; std::cout << "Comentario: "; std::getline(std::cin >> std::ws, comentario);

    int id = nextIdForAvaliacoes(avaliacoes);
    avaliacoes.emplace_back(id, profId, "PROFESSOR", nota, comentario);
    std::cout << "Avaliação anônima de professor registrada.\n";
}

void SistemaAvaliacao::avaliarTurma() {
    if (turmas.empty()) { std::cout << "Nenhuma turma.\n"; return; }
    std::cout << "Turmas:\n";
    for (auto &t : turmas) std::cout << t.getId() << " - disciplina " << t.getDisciplinaId() << " turma " << t.getCodigoTurma() << '\n';
    int turmaId; std::cout << "Escolha ID da turma: "; std::cin >> turmaId;
    int nota; std::cout << "Nota (0-10): "; std::cin >> nota;
    std::string comentario; std::cout << "Comentario: "; std::getline(std::cin >> std::ws, comentario);

    int id = nextIdForAvaliacoes(avaliacoes);
    avaliacoes.emplace_back(id, turmaId, "TURMA", nota, comentario);
    std::cout << "Avaliação anônima de turma registrada.\n";
}

// ----- LISTAGEM -----
void SistemaAvaliacao::listarAvaliacoes(const std::string &tipo) {
    if (tipo == "GERAL") {
        std::cout << "=== Todas avaliações ===\n";
        for (auto &a : avaliacoes) {
            std::cout << "ID: " << a.getId() << " | Tipo: " << a.getTipo() << " | Alvo: " << a.getAlvoId()
                      << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
        return;
    }
    // filtrar por tipo
    std::cout << "=== Avaliações do tipo " << tipo << " ===\n";
    for (auto &a : avaliacoes) {
        if (a.getTipo() == tipo) {
            std::cout << "ID: " << a.getId() << " | Alvo: " << a.getAlvoId()
                      << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
    }
}

// ----- SALVAR -----
void SistemaAvaliacao::salvarTudo() {
    // NOTA: salvarUsuarios precisa de getSenha() no header para escrever senhas corretas.
    arquivo.salvarUsuarios(usuarios);
    arquivo.salvarDisciplinas(disciplinas);
    arquivo.salvarTurmas(turmas);
    arquivo.salvarAvaliacoes(avaliacoes);
    std::cout << "Dados salvos em disco (data/)\n";
}