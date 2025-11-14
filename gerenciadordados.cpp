#include "gerenciadordados.hpp"
#include <iostream>
#include <stdexcept>

GerenciadorDados::GerenciadorDados() {
    try {
        carregar_usuarios();
        carregar_disciplinas();
        carregar_turmas();
        carregar_avaliacoes(); 
    } catch (const std::exception& e) {
        std::cerr << "AVISO: Falha ao carregar alguns dados. Iniciando com dados vazios." << std::endl;
    }
}

GerenciadorDados::~GerenciadorDados() {
    try {
        salvar_usuarios();
        salvar_disciplinas();
        salvar_turmas();
        salvar_avaliacoes();
    } catch (const std::exception& e) {
        std::cerr << "ERRO FATAL: Falha ao salvar os dados. Algumas alteracoes podem ter sido perdidas." << std::endl;
    }
}

void GerenciadorDados::salvar_avaliacoes() const {
    std::ofstream arquivo(ARQ_AVALIACOES); 
    if (arquivo.is_open()) {
        for (const auto& avaliacao : _avaliacoes) {
             
        }
        arquivo.close();
    } else {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo " << ARQ_AVALIACOES << " para salvar." << std::endl;
    }
}

void GerenciadorDados::carregar_avaliacoes() {
    std::ifstream arquivo(ARQ_AVALIACOES);
    _avaliacoes.clear(); 

    if (arquivo.is_open()) {
        while (!arquivo.eof()) {
            try {
                
                
            } catch (const std::exception& e) {
                break; 
            }
        }
        arquivo.close();
    } 
}

void GerenciadorDados::salvar_usuarios() const {
    std::ofstream arquivo(ARQ_USUARIOS);
    if (arquivo.is_open()) {
        for (const auto& usuario : _usuarios) {
        }
        arquivo.close();
    } else {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo " << ARQ_USUARIOS << " para salvar." << std::endl;
    }
}

void GerenciadorDados::carregar_usuarios() {
    std::ifstream arquivo(ARQ_USUARIOS);
    _usuarios.clear(); 
    if (arquivo.is_open()) {
        while (!arquivo.eof()) {
            try {
                
                
            } catch (const std::exception& e) {
                break; 
            }
        }
        arquivo.close();
    }
}

void GerenciadorDados::salvar_disciplinas() const {
    std::ofstream arquivo(ARQ_DISCIPLINAS);
    if (arquivo.is_open()) {
        for (const auto& disciplina : _disciplinas) {
        }
        arquivo.close();
    } else {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo " << ARQ_DISCIPLINAS << " para salvar." << std::endl;
    }
}

void GerenciadorDados::carregar_disciplinas() {
    std::ifstream arquivo(ARQ_DISCIPLINAS);
    _disciplinas.clear();
}

void GerenciadorDados::salvar_turmas() const {
    std::ofstream arquivo(ARQ_TURMAS);
    if (arquivo.is_open()) {
        for (const auto& turma : _turmas) {
        }
        arquivo.close();
    } else {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo " << ARQ_TURMAS << " para salvar." << std::endl;
    }
}

void GerenciadorDados::carregar_turmas() {
    std::ifstream arquivo(ARQ_TURMAS);
    _turmas.clear();
}

void GerenciadorDados::adicionar_avaliacao(const Avaliacao& avaliacao) {
    _avaliacoes.push_back(avaliacao);
}

const std::vector<Avaliacao>& GerenciadorDados::get_avaliacoes() const {
    return _avaliacoes;
}

void GerenciadorDados::adicionar_usuario(const Usuario& usuario) {
    _usuarios.push_back(usuario);
}

const std::vector<Usuario>& GerenciadorDados::get_usuarios() const {
    return _usuarios;
}

Usuario* GerenciadorDados::buscar_usuario_por_ID(int ID) {
    for (auto& usuario : _usuarios) {
        if (usuario.get_ID() == ID) {
            return &usuario; 
        }
    }
    return nullptr; 
}

void GerenciadorDados::adicionar_disciplina(const Disciplina& disciplina) {
    _disciplinas.push_back(disciplina);
}

const std::vector<Disciplina>& GerenciadorDados::get_disciplinas() const {
    return _disciplinas;
}

void GerenciadorDados::adicionar_turma(const Turma& turma) {
    _turmas.push_back(turma);
}

const std::vector<Turma>& GerenciadorDados::get_turmas() const {
    return _turmas;
}