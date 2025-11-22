#include "FileManager.hpp"
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

FileManager::FileManager() {
    // assegura que a pasta data exista
    std::filesystem::create_directories("data");
}

//USUARIOS
void FileManager::carregarUsuarios(std::vector<Usuario>& usuarios) {
    std::ifstream in("usuarios.txt");
    if (!in.is_open()) {
        std::cout << "Nenhum arquivo de usuarios encontrado. Criando novo..." << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(in, linha)) {
        if (linha.empty()) continue;

        std::stringstream ss(linha);
        std::string idStr, nome, email, hash, tipo;

        std::getline(ss, idStr, ';');
        std::getline(ss, nome, ';');
        std::getline(ss, email, ';');
        std::getline(ss, hash, ';');
        std::getline(ss, tipo, ';');

        int id = std::stoi(idStr);
        Usuario u = nullptr;

      
        // Instanciar com o construtor de HASH
       
        if (tipo == "Aluno") {
            u = new Aluno(id, nome, email, hash, tipo, true);
        }
        else if (tipo == "Professor") {
            u = new Professor(id, nome, email, hash, tipo, true);
        }
        else if (tipo == "CoordenadorCurso") {
            u = new CoordenadorCurso(id, nome, email, hash, tipo, true);
        }
        else if (tipo == "CoordenadorDisciplina") {
            u = new CoordenadorDisciplina(id, nome, email, hash, tipo, true);
        }
        else {
            std::cout << "Tipo de usuario desconhecido: " << tipo << std::endl;
            continue;
        }

        _usuarios.push_back(u);
    }

    in.close();
}

void FileManager::salvarUsuarios(const std::vector<Usuario*>& lista) {
    std::ofstream out("data/usuarios.txt", std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir data/usuarios.txt para escrita\n";
        return;
    }
    for (auto u : lista) {
        out << u->getId() << ";"
        << u->getNome() << ";"
        << u->getEmail() << ";"
        << u->getHash() << ";"    
        << u->getTipo() << "\n";
            
    }
    out.close();
}

// ----------------- DISCIPLINAS -----------------
std::vector<Disciplina> FileManager::carregarDisciplinas() {
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

void FileManager::salvarDisciplinas(const std::vector<Disciplina>& lista) {
    std::ofstream out("data/disciplinas.txt", std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir data/disciplinas.txt\n";
        return;
    }
    for (const auto &d : lista) {
        out << d.getId() << ';' << d.getCodigo() << ';' << d.getNome() << ';' << d.getProfessorId() << ';' << d.getCoordDiscId() << '\n';
    }
    out.close();
}

// ----------------- TURMAS -----------------
std::vector<Turma> FileManager::carregarTurmas() {
    std::vector<Turma> res;
    std::ifstream in("data/turmas.txt");
    if (!in.is_open()) {
        std::ofstream out("data/turmas.txt", std::ios::app);
        out.close();
        return res;
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
        res.emplace_back(id, discId, codigo, profId);
    }
    in.close();
    return res;
}

void FileManager::salvarTurmas(const std::vector<Turma>& lista) {
    std::ofstream out("data/turmas.txt", std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir data/turmas.txt\n";
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
std::vector<Avaliacao> FileManager::carregarAvaliacoes() {
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
        res.emplace_back(id, alvoId, tipo, nota, comentario);
    }
    in.close();
    return res;
}

void FileManager::salvarAvaliacoes(const std::vector<Avaliacao>& lista) {
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
            << a.getComentario() << '\n';
    }
    out.close();
}
