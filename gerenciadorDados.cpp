#include "gerenciadorDados.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

const std::string ARQUIVO_MATRICULAS = "data/matriculas.txt";

static std::vector<std::string> dividirLinha(const std::string &linha, char delim=';') {
    std::vector<std::string> partes;
    std::stringstream fluxo(linha);
    std::string campo;
    while (std::getline(fluxo, campo, delim)) partes.push_back(campo);
    return partes;
}

GerenciadorDados::GerenciadorDados() {
    std::filesystem::create_directories("data");
}

GerenciadorDados::~GerenciadorDados() {}

//USUARIOS
void GerenciadorDados::carregarUsuarios(std::vector<Usuario*>& usuarios) {
    std::ifstream arquivoEntrada("data/usuarios.txt");
    if (!arquivoEntrada.is_open()) {
        std::cout << "Nenhum arquivo de usuarios encontrado. Criando novo..." << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(arquivoEntrada, linha)) {
        if (linha.empty()) continue;

        auto campos = dividirLinha(linha, ';');
        if (campos.size() < 5) {
            std::cerr << "ERRO: Linha de usuario invalida: " << linha << std::endl;
            continue;
        }
        
        int id = std::stoi(campos[0]);
        std::string nome = campos[1];
        std::string email = campos[2];
        std::string hash = campos[3];
        std::string tipo = campos[4];

        Usuario* u= nullptr;
       
        if (tipo == "ALUNO") {
            u = new Aluno(id, nome, email, hash, tipo, true);
        }
        else if (tipo == "PROFESSOR") {
            u = new Professor(id, nome, email, hash, tipo, true);
        }
        else if (tipo == "COORDENADOR_DO_CURSO") {
            u = new CoordenadorCurso(id, nome, email, hash, tipo, true);
        }
        else if (tipo == "COORDENADOR_DISCIPLINA") {
            u = new CoordenadorDisciplina(id, nome, email, hash, tipo, true);
        }
        else {
            std::cout << "Tipo de usuario desconhecido: " << tipo << std::endl;
            continue;
        }

        usuarios.push_back(u);
    }

    arquivoEntrada.close();
}

void GerenciadorDados::salvarUsuarios(const std::vector<Usuario*>& lista) {
    std::ofstream arquivoSaida("data/usuarios.txt", std::ios::trunc);

    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir usuarios.txt para escrita\n";
        return;
    }

    for (auto u : lista) {
        arquivoSaida << u->getId() << ";" << u->getNome() << ";" << u->getemail() << ";" << u->getHash() << ";" << u->getTipo() << "\n";
            
    }
    arquivoSaida.close();
}

void GerenciadorDados::carregarMatriculas(std::vector<Usuario*>& usuarios, std::vector<Turma>& turmas) {
    std::ifstream arquivo(ARQUIVO_MATRICULAS);

    if (!arquivo.is_open()) {
        std::cerr << "AVISO: Arquivo de matriculas nao encontrado. Criando novo arquivo na proxima gravacao.\n";
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        auto campos = dividirLinha(linha, ';');
        if (campos.size() != 2) {
            std::cerr << "AVISO: Linha ignorada em matriculas.txt: Formato invalido: " << linha << "\n";
            continue;
        }

        try {
            int alunoId = std::stoi(campos[0]);
            int turmaId = std::stoi(campos[1]);
            
            Usuario* aluno = nullptr;
            for (auto* u : usuarios) {
                if (u->getId() == alunoId && u->getTipo() == "ALUNO") {
                    aluno = u;
                    break;
                }
            }

            Turma* turma = nullptr;
            for (auto& t : turmas) {
                if (t.getId() == turmaId) {
                    turma = &t;
                    break;
                }
            }

            // Apenas matricula se o usuário for um ALUNO e a turma existir
            if (aluno && turma) {
                aluno->addTurma(turma); 
            } else {
                 std::cerr << "AVISO: Matricula (Aluno ID:" << alunoId << ", Turma ID:" << turmaId << ") nao pode ser carregada (usuario/turma nao existe ou usuario nao e' aluno).\n";
            }
        } catch (const std::exception& e) {
             std::cerr << "ERRO de parsing em matriculas.txt: " << linha << "\n";
        }
    }
    arquivo.close();
    std::cout << "Matriculas carregadas.\n";
}

void GerenciadorDados::salvarMatriculas(const std::vector<Usuario*>& lista) const {
    std::ofstream arquivo(ARQUIVO_MATRICULAS, std::ios::trunc);

    if (!arquivo.is_open()) {
        std::cerr << "ERRO FATAL: Nao foi possivel abrir o arquivo para salvar as matriculas: " << ARQUIVO_MATRICULAS << "\n";
        return;
    }
    
    // Itera sobre todos os usuários e salva apenas as matrículas dos alunos.
    for (const auto* u : lista) {
        if (u->getTipo() == "ALUNO") {
            for (const auto* turma : u->getMinhasDisciplinas()) {
                // Formato: ID_ALUNO;ID_TURMA
                arquivo << u->getId() << ";" << turma->getId() << "\n";
            }
        }
    }

    arquivo.close();
}


// ----------------- DISCIPLINAS -----------------
std::vector<Disciplina> GerenciadorDados::carregarDisciplinas() {

    std::vector<Disciplina> disciplinas;
    std::ifstream arquivoEntrada("data/disciplinas.txt");

    if (!arquivoEntrada.is_open()) {
        std::ofstream arquivoSaida("data/disciplinas.txt", std::ios::app);
        arquivoSaida.close();
        return disciplinas;
    }

    std::string linha;
    while (std::getline(arquivoEntrada, linha)) {
        if (linha.empty()) continue;

        auto campos = dividirLinha(linha, ';');
        if (campos.size() < 5) continue;

        int id = std::stoi(campos[0]);
        std::string codigo = campos[1];
        std::string nome = campos[2];
        int professorId = std::stoi(campos[3]);
        int coordDiscId = std::stoi(campos[4]);
        disciplinas.emplace_back(id, codigo, nome, professorId, coordDiscId);
    }
    arquivoEntrada.close();
    return disciplinas;
}

void GerenciadorDados::salvarDisciplinas(const std::vector<Disciplina>& lista) {

    std::ofstream arquivoSaida("data/disciplinas.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir disciplinas.txt\n";
        return;
    }

    for (const auto &d : lista) {
        arquivoSaida << d.getId() << ';' << d.getCodigo() << ';' << d.getNome() << ';' << d.getProfessorId() << ';' << d.getCoordenadorDiscId() << '\n';
    }

    arquivoSaida.close();
}



// ----------------- TURMAS -----------------
std::vector<Turma> GerenciadorDados::carregarTurmas() {
    std::vector<Turma> turma;
    std::ifstream arquivoEntrada("data/turmas.txt");

    if (!arquivoEntrada.is_open()) {
        std::ofstream arquivoSaida("data/turmas.txt", std::ios::app);
        arquivoSaida.close();
        return turma;
    }

    std::string linha;
    while (std::getline(arquivoEntrada, linha)) {
        if (linha.empty()) continue;

        auto campos = dividirLinha(linha, ';');
        if (campos.size() < 4) continue;

        int id = std::stoi(campos[0]);
        int discId = std::stoi(campos[1]);
        std::string codigo = campos[2];
        int profId = std::stoi(campos[3]);

        turma.emplace_back(id, discId, codigo, profId);
    }

    arquivoEntrada.close();
    return turma;
}

void GerenciadorDados::salvarTurmas(const std::vector<Turma>& lista) {
    std::ofstream arquivoSaida("data/turmas.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir turmas.txt\n";
        return;
    }

    for (const auto &t : lista) {
        arquivoSaida << t.getId() << ';' << t.getDisciplinaId() << ';' << t.getCodigoTurma() << ';' << t.getProfessorId() << '\n';
    }
    arquivoSaida.close();
}



// ----------------- AVALIAÇÕES -----------------
std::vector<Avaliacao> GerenciadorDados::carregarAvaliacoes() {
    std::vector<Avaliacao> avaliacoes;
    std::ifstream arquivoEntrada("data/avaliacoes.txt");

    if (!arquivoEntrada.is_open()) {
        std::ofstream arquivoSaida("data/avaliacoes.txt", std::ios::app);
        arquivoSaida.close();
        return avaliacoes;
    }

    std::string linha;
    while (std::getline(arquivoEntrada, linha)) {
        if (linha.empty()) continue;

        auto campos = dividirLinha(linha, ';');
        if (campos.size() < 5) continue;

        int id = std::stoi(campos[0]);
        int alvoId = std::stoi(campos[1]);
        std::string tipo = campos[2];
        int nota = std::stoi(campos[3]);
        std::string comentario = campos[4];
        std::string data = campos[5];

        avaliacoes.emplace_back(id, alvoId, tipo, nota, comentario, data);
    }

    arquivoEntrada.close();
    return avaliacoes;
}



void GerenciadorDados::salvarAvaliacoes(const std::vector<Avaliacao>& lista) {
    std::ofstream arquivoSaida("data/avaliacoes.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir data/avaliacoes.txt\n";
        return;
    }

    for (const auto &a : lista) {
        arquivoSaida << a.getId() << ';' << a.getAlvoId() << ';' << a.getTipo() << ';' << a.getNota() << ';' << a.getComentario() << ';'  << getDataAtual() << '\n';
    }
    arquivoSaida.close();
}