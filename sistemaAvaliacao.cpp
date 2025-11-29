#include "sistemaAvaliacao.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

// carrega tudo do GerenciadorDados
SistemaAvaliacao::SistemaAvaliacao() {
    _usuarios = arquivo.carregarUsuarios();
    _disciplinas = arquivo.carregarDisciplinas();
    _turmas = arquivo.carregarTurmas();
    _avaliacoes = arquivo.carregarAvaliacoes();


    // Se nao existir nenhum coordenador do curso, cria um padrao
    bool temCoord = false;
    for (auto u : _usuarios) if (u->getTipo() == "COORDENADOR_DO_CURSO") { 
        temCoord = true; 
        break; 
    }
    if (!temCoord) {
        // adiciona um administrador padrao
        CoordenadorCurso *admin = new CoordenadorCurso(1, "Admin Curso", "admin@ufv.br", "admin123");
        _usuarios.insert(_usuarios.begin(), admin);
    }
}

// libera ponteiros de _usuarios
SistemaAvaliacao::~SistemaAvaliacao() {
    for (auto u : _usuarios) delete u;
}

// gerar novo id para cada tipo

    static int ProximoIdUsuarios(const std::vector<Usuario*> &v) {
        int ultimoId = 0;
        for (auto u : v) if (u->getId() > ultimoId) ultimoId = u->getId();
        return ultimoId + 1;
    }

    static int ProximoIdDisciplinas(const std::vector<Disciplina> &v) {
        int ultimoId = 0;
        for (auto &d : v) if (d.getId() > ultimoId) ultimoId = d.getId();
        return ultimoId + 1;
    }

    static int ProximoIdTurmas(const std::vector<Turma> &v) {
        int ultimoId = 0;
        for (auto &t : v) if (t.getId() > ultimoId) ultimoId = t.getId();
        return ultimoId + 1;
    }

    static int ProximoIdAvaliacoes(const std::vector<Avaliacao> &v) {
        int ultimoId = 0;
        for (auto &a : v) if (a.getId() > ultimoId) ultimoId = a.getId();
        return ultimoId + 1;
    }

// get
std::vector<Usuario*>& SistemaAvaliacao::getUsuarios() {
    return _usuarios;
}

// CADASTROS

void SistemaAvaliacao::cadastrarUsuario(const std::string &tipo) {

    std::string nome, email, senha;
    std::cout << "Nome: ";
    std::getline(std::cin >> std::ws, nome); //usamos std::ws para descartar espaco em branco, quebra de linha e tabulacao
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Senha: ";
    std::cin >> senha;

    int id = ProximoIdUsuarios(_usuarios);

    Usuario* novo = nullptr;

        if (tipo == "ALUNO") novo = new Aluno(id, nome, email, senha);
        else if (tipo == "PROFESSOR") novo = new Professor(id, nome, email, senha);
        else if (tipo == "COORDENADOR_DISCIPLINA") novo = new CoordenadorDisciplina(id, nome, email, senha);
        else if (tipo == "COORDENADOR_DO_CURSO") novo = new CoordenadorCurso(id, nome, email, senha);
        else {
            std::cerr << "ERRO: Voce nao se encaixa em nenhum tipo de usuario dessa organizacao.\n";
            return;
    }

    _usuarios.push_back(novo);
    std::cout << "Usuario cadastrado com ID: " << id << "\n";
}

void SistemaAvaliacao::cadastrarDisciplina() {

    std::string codigo, nome;
    int profId, coordId;

    std::cout << "Codigo da disciplina (ex: INF112): ";
    std::cin >> codigo;

    std::cout << "Nome da disciplina: ";
    std::getline(std::cin >> std::ws, nome);

    // listar professores disponiveis
    std::cout << "Professores disponiveis:\n";
    for (auto u : _usuarios) if (u->getTipo() == "PROFESSOR")
        std::cout << u->getId() << " - " << u->getNome() << '\n';

        std::cout << "Escolha o ID do professor responsavel: ";
        std::cin >> profId;

    std::cout << "Coordenadores de disciplina disponiveis:\n";
    for (auto u : _usuarios) if (u->getTipo() == "COORDENADOR_DISCIPLINA")
        std::cout << u->getId() << " - " << u->getNome() << '\n';

        std::cout << "Escolha o ID do coordenador da disciplina: ";
        std::cin >> coordId;

    int id = ProximoIdDisciplinas(_disciplinas);
    _disciplinas.emplace_back(id, codigo, nome, profId, coordId);
    std::cout << "Disciplina cadastrada (ID=" << id << ")\n";
}

void SistemaAvaliacao::cadastrarTurma() {

    int disciplinaId;
    std::string codigoTurma;
    int professorId;

    std::cout << "Disciplinas disponiveis:\n";
    for (auto &d : _disciplinas) std::cout << d.getId() << " - " << d.getCodigo() << " " << d.getNome() << '\n';

    std::cout << "Escolha ID da disciplina: ";
    std::cin >> disciplinaId;

    std::cout << "Codigo da turma (ex: 1): ";
    std::cin >> codigoTurma;

    std::cout << "Professores disponiveis:\n";
    for (auto u : _usuarios) if (u->getTipo() == "PROFESSOR")
        std::cout << u->getId() << " - " << u->getNome() << '\n';
    std::cout << "Escolha ID do professor para a turma: ";
    std::cin >> professorId;

    int id = ProximoIdTurmas(_turmas);
    _turmas.emplace_back(id, disciplinaId, codigoTurma, professorId);
    std::cout << "Turma cadastrada (ID=" << id << ")\n";
}

// AVALIACOES

std::string SistemaAvaliacao::getDataAtual() {
    time_t agora = time(nullptr);
    tm *ltm = localtime(&agora);

    std::ostringstream oss;
    oss << std::put_time(ltm, "%Y-%m-%d");
    return oss.str();
}

void SistemaAvaliacao::avaliarDisciplina() {

    if (_disciplinas.empty()) {
        std::cout << "Nenhuma disciplina cadastrada.\n";
        return;
    }
    std::cout << "Disciplinas:\n";
    for (auto &d : _disciplinas) 
    std::cout << d.getId() << " - " << d.getCodigo() << " " << d.getNome() << '\n';

    int idDisc;
    std::cout << "Escolha ID da disciplina: ";
    std::cin >> idDisc;

    int nota;
    std::cout << "Nota (0-10): ";
    std::cin >> nota;

    std::string comentario;
    std::cout << "Comentario: ";
    std::getline(std::cin >> std::ws, comentario);

    int id = ProximoIdAvaliacoes(_avaliacoes);
    _avaliacoes.emplace_back(id, idDisc, "DISCIPLINA", nota, comentario,getDataAtual());
    std::cout << "Avaliacao de disciplina registrada.\n";
}

void SistemaAvaliacao::avaliarProfessor() {

    std::cout << "Professores:\n";
    for (auto u : _usuarios) if (u->getTipo() == "PROFESSOR") 
    std::cout << u->getId() << " - " << u->getNome() << '\n';

    int profId; 
    std::cout << "Escolha ID do professor: "; 
    std::cin >> profId;

    int nota; 
    std::cout << "Nota (0-10): "; 
    std::cin >> nota;

    std::string comentario; 
    std::cout << "Comentario: "; 
    std::getline(std::cin >> std::ws, comentario);

    int id = ProximoIdAvaliacoes(_avaliacoes);
    _avaliacoes.emplace_back(id, profId, "PROFESSOR", nota, comentario, getDataAtual());
    std::cout << "Avaliacao de professor registrada.\n";
}

void SistemaAvaliacao::avaliarTurma() {

    if (_turmas.empty()) { 
        std::cout << "Nenhuma turma.\n"; 
        return; 
    }

    std::cout << "Turmas:\n";
    for (auto &t : _turmas)
        std::cout << t.getId() << " - disciplina " << t.getDisciplinaId() << " turma " << t.getCodigoTurma() << '\n';

    int turmaId; 
    std::cout << "Escolha ID da turma: "; 
    std::cin >> turmaId;

    int nota; 
    std::cout << "Nota (0-10): "; 
    std::cin >> nota;

    std::string comentario; 
    std::cout << "Comentario: "; 
    std::getline(std::cin >> std::ws, comentario);

    int id = ProximoIdAvaliacoes(_avaliacoes);
    _avaliacoes.emplace_back(id, turmaId, "TURMA", nota, comentario, getDataAtual());
    std::cout << "Avaliacao de turma registrada.\n";
}

//LISTAGEM
void SistemaAvaliacao::listarAvaliacoes(const std::string &tipo) {

    if (tipo == "GERAL") {
        std::cout << " Avaliacoes disponiveis : \n";

        for (auto &a : _avaliacoes) {
            std::cout << "ID: " << a.getId() << " | Tipo: " << a.getTipo() << " | Alvo: " << a.getAlvoId() << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
        return;
    }

    // filtrar por tipo
    std::cout << " Avaliacoes do tipo " << tipo << std::endl;
    for (auto &a : _avaliacoes) {
        if (a.getTipo() == tipo) {
            std::cout << "ID: " << a.getId() << " | Alvo: " << a.getAlvoId() << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
    }
}

// SALVAR
void SistemaAvaliacao::salvarTudo() {
    // NOTA: salvarUsuarios precisa de getSenha() no header para escrever senhas corretas.
    arquivo.salvarUsuarios(_usuarios);
    arquivo.salvarDisciplinas(_disciplinas);
    arquivo.salvarTurmas(_turmas);
    arquivo.salvarAvaliacoes(_avaliacoes);
    std::cout << "Dados salvos em disco (data/)\n";
}