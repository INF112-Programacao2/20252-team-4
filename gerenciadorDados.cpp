#include "gerenciadorDados.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

// helper: split line by delimiter
static std::vector<std::string> splitLine(const std::string &line, char delim=';') {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, delim)) parts.push_back(item);
    return parts;
}

GerenciadorDados::GerenciadorDados() {
    // assegura que a pasta data exista
    std::filesystem::create_directories("data");
}

GerenciadorDados::~GerenciadorDados() {}

//USUARIOS
void GerenciadorDados::carregarUsuarios(std::vector<Usuario*>& usuarios) {
    std::ifstream in("data/usuarios.txt");
    if (!in.is_open()) {
        std::cout << "Nenhum arquivo de usuarios encontrado. Criando novo..." << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(in, linha)) {
        if (linha.empty()) continue;

        std::stringstream ss(linha);
        std::string idStr, nome, matricula, hash, tipo;

        std::getline(ss, idStr, ';');
        std::getline(ss, nome, ';');
        std::getline(ss, matricula, ';');
        std::getline(ss, hash, ';');
        std::getline(ss, tipo, ';');

        int id = std::stoi(idStr);
        Usuario* u= nullptr;
        

      
        // Instanciar com o construtor de HASH
       
        if (tipo == "ALUNO") {
            u = new Aluno(id, nome, matricula, hash, tipo, true);
        }
        else if (tipo == "PROFESSOR") {
            u = new Professor(id, nome, matricula, hash, tipo, true);
        }
        else if (tipo == "COORDENADOR_DO_CURSO") {
            u = new CoordenadorCurso(id, nome, matricula, hash, tipo, true);
        }
        else if (tipo == "COORDENADOR_DISCIPLINA") {
            u = new CoordenadorDisciplina(id, nome, matricula, hash, tipo, true);
        }
        else {
            std::cout << "Tipo de usuario desconhecido: " << tipo << std::endl;
            continue;
        }

        usuarios.push_back(u);
    }

    in.close();
}

void GerenciadorDados::salvarUsuarios(const std::vector<Usuario*>& lista) {
    std::ofstream out("data/usuarios.txt", std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir usuarios.txt para escrita\n";
        return;
    }
    for (auto u : lista) {
        out << u->getId() << ";"
        << u->getNome() << ";"
        << u->getmatricula() << ";"
        << u->getHash() << ";"    
        << u->getTipo() << "\n";
            
    }
    out.close();
}

// ----------------- DISCIPLINAS -----------------
std::vector<Disciplina> GerenciadorDados::carregarDisciplinas() {
    std::vector<Disciplina> res;
    std::ifstream in("data/disciplinas.txt");
    if (!in.is_open()) {
        std::ofstream out("data/disciplinas.txt", std::ios::app);
        out.close();
        return res;
    }
    std::string linha;
    while (std::getline(in, linha)) {
        if (linha.empty()) continue;
        auto campos = splitLine(linha, ';');
        if (campos.size() < 5) continue;
        int id = std::stoi(campos[0]);
        std::string codigo = campos[1];
        std::string nome = campos[2];
        int professorId = std::stoi(campos[3]);
        int coordDiscId = std::stoi(campos[4]);
        res.emplace_back(id, codigo, nome, professorId, coordDiscId);
    }
    in.close();
    return res;
}

void GerenciadorDados::salvarDisciplinas(const std::vector<Disciplina>& lista) {
    std::ofstream out("data/disciplinas.txt", std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir disciplinas.txt\n";
        return;
    }
    for (const auto &d : lista) {
        out << d.getId() << ';' << d.getCodigo() << ';' << d.getNome() << ';' << d.getProfessorId() << ';' << d.getCoordenadorDiscId() << '\n';
    }
    out.close();
}

// ----------------- TURMAS -----------------
std::vector<Turma> GerenciadorDados::carregarTurmas() {
    std::vector<Turma> turma;
    std::ifstream in("data/turmas.txt");
    if (!in.is_open()) {
        std::ofstream out("data/turmas.txt", std::ios::app);
        out.close();
        return turma;
    }
    std::string linha;
    while (std::getline(in, linha)) {
        if (linha.empty()) continue;
        auto campos = splitLine(linha, ';');
        if (campos.size() < 4) continue;
        int id = std::stoi(campos[0]);
        int discId = std::stoi(campos[1]);
        std::string codigo = campos[2];
        int profId = std::stoi(campos[3]);
        turma.emplace_back(id, discId, codigo, profId);
    }
    in.close();
    return turma;
}

void GerenciadorDados::salvarTurmas(const std::vector<Turma>& lista) {
    std::ofstream out("data/turmas.txt", std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir turmas.txt\n";
        return;
    }
    for (const auto &t : lista) {
        out << t.getId() << ';'
            << t.getDisciplinaId() << ';'
            << t.getCodigoTurma() << ';'
            << t.getProfessorId() << '\n';
    }
    out.close();
}

// ----------------- AVALIAÇÕES -----------------
std::vector<Avaliacao> GerenciadorDados::carregarAvaliacoes() {
    std::vector<Avaliacao> res;
    std::ifstream in("data/avaliacoes.txt");
    if (!in.is_open()) {
        std::ofstream out("data/avaliacoes.txt", std::ios::app);
        out.close();
        return res;
    }
    std::string linha;
    while (std::getline(in, linha)) {
        if (linha.empty()) continue;
        auto campos = splitLine(linha, ';');
        if (campos.size() < 5) continue;
        int id = std::stoi(campos[0]);
        int alvoId = std::stoi(campos[1]);
        std::string tipo = campos[2];
        int nota = std::stoi(campos[3]);
        std::string comentario = campos[4];
        std::string data = campos[5];
        res.emplace_back(id, alvoId, tipo, nota, comentario, data);
    }
    in.close();
    return res;
}

void GerenciadorDados::salvarAvaliacoes(const std::vector<Avaliacao>& lista) {
    std::ofstream out("data/avaliacoes.txt", std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir data/avaliacoes.txt\n";
        return;
    }
    for (const auto &a : lista) {
        out << a.getId() << ';'
            << a.getAlvoId() << ';'
            << a.getTipo() << ';'
            << a.getNota() << ';'
            << a.getComentario() << ';' 
            << getDataAtual() << '\n';
    }
    out.close();
}
