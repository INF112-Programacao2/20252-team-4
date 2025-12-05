#include "gerenciadorDados.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

//define o caminho do arquivo de matriculas
const std::string ARQUIVO_MATRICULAS = "data/matriculas.txt";

//funcao auxiliar para criar uma pasta data para armazenar os arquivos
GerenciadorDados::GerenciadorDados() {
    std::filesystem::create_directories("data");
}

GerenciadorDados::~GerenciadorDados() {}

//carrega os usuarios do arquivo
void GerenciadorDados::carregarUsuarios(std::vector<Usuario*>& usuarios) {
    std::ifstream arquivoEntrada("data/usuarios.txt");
    if (!arquivoEntrada.is_open()) { //se o arquivo nao existir, cria um novo
        std::cout << "Nenhum arquivo de usuarios encontrado. Criando novo..." << std::endl;
        return;
    }

    std::string linha; 
    while (std::getline(arquivoEntrada, linha)) {
        if (linha.empty()) 
        continue;

        auto campos = _utilidades.dividirLinha(linha, ';'); //divide a linha em campos
        if (campos.size() < 5) {
            std::cerr << "ERRO: Linha de usuario invalida: " << linha << std::endl;
            continue;
        }
        
        int id = std::stoi(campos[0]); //converte o id para inteiro
        std::string nome = campos[1];
        std::string email = campos[2];
        std::string hash = campos[3];
        std::string tipo = campos[4];

        Usuario* u= nullptr; //ponteiro para usuario, inicialmente nulo

        //cria o usuario com base no tipo
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

    arquivoEntrada.close(); //fecha o arquivo de entrada
}

//salva os usuarios no arquivo
void GerenciadorDados::salvarUsuarios(const std::vector<Usuario*>& lista) {
    std::ofstream arquivoSaida("data/usuarios.txt", std::ios::trunc); //abre o arquivo para escrita, apagando o conteudo anterior, usa std::trunc para isso

    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir usuarios.txt para escrita\n";
        return;
    }

    //escreve cada usuario no arquivo
    for (auto u : lista) {
        arquivoSaida << u->getId() << ";" << u->getNome() << ";" << u->getemail() << ";" << u->getHash() << ";" << u->getTipo() << "\n";
            
    }
    arquivoSaida.close();
}


//carrega as matriculas dos alunos nas turmas
void GerenciadorDados::carregarMatriculas(std::vector<Usuario*>& usuarios, std::vector<Turma>& turmas) {
    std::ifstream arquivo(ARQUIVO_MATRICULAS); //abre o arquivo de matriculas

    if (!arquivo.is_open()) { //se o arquivo nao existir, cria um novo
        std::cerr << "AVISO: Arquivo de matriculas nao encontrado. Criando novo arquivo na proxima gravacao.\n";
        return;
    }

    //mesma coisa, repete o processo de leitura linha por linha
    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) 
        continue;

        auto campos = _utilidades.dividirLinha(linha, ';');
        if (campos.size() != 2) {
            std::cerr << "AVISO: Linha ignorada em matriculas.txt: Formato invalido: " << linha << "\n";
            continue;
        }

        try {
            int alunoId = std::stoi(campos[0]);//converte o id do aluno para inteiro
            int turmaId = std::stoi(campos[1]); 
            
            Usuario* aluno = nullptr;
            for (auto* u : usuarios) {
                if (u->getId() == alunoId && u->getTipo() == "ALUNO") {
                    aluno = u;
                    break; //encontra o aluno correspondente
                }
            }

            Turma* turma = nullptr;
            for (auto& t : turmas) {
                if (t.getId() == turmaId) {
                    turma = &t;
                    break; //encontra a turma correspondente
                }
            }

            // Apenas matricula se o usuario for um ALUNO e a turma existir
            if (aluno && turma) {
                aluno->addTurma(turma); 
            } else {
                 std::cerr << "AVISO: Matricula (Aluno ID:" << alunoId << ", Turma ID:" << turmaId << ") nao pode ser carregada (usuario/turma nao existe ou usuario nao e' aluno).\n";
            }
        } catch (const std::exception& e) {
             std::cerr << "ERRO de conversao em matriculas.txt: " << linha << "\n"; //erro ao converter id para inteiro
        }
    }
    arquivo.close(); //fecha o arquivo de matriculas
}

void GerenciadorDados::salvarMatriculas(const std::vector<Usuario*>& lista) const {
    std::ofstream arquivo(ARQUIVO_MATRICULAS, std::ios::trunc); //abre o arquivo para escrita, apagando o conteudo anterior

    if (!arquivo.is_open()) {
        std::cerr << "ERRO FATAL: Nao foi possivel abrir o arquivo para salvar as matriculas: " << ARQUIVO_MATRICULAS << "\n";
        return;
    }
    
    //para cada usuario na lista, se for aluno, salva suas turmas
    for (const auto* u : lista) {
        if (u->getTipo() == "ALUNO") {
            for (const auto* turma : u->getMinhasDisciplinas()) {
                arquivo << u->getId() << ";" << turma->getId() << "\n";
            }
        }
    } 

    arquivo.close();
}

//carrega as disciplinas do arquivo
std::vector<Disciplina> GerenciadorDados::carregarDisciplinas() {

    std::vector<Disciplina> disciplinas; //vetor para armazenar as disciplinas
    std::ifstream arquivoEntrada("data/disciplinas.txt");

    if (!arquivoEntrada.is_open()) {
        std::ofstream arquivoSaida("data/disciplinas.txt", std::ios::app);
        arquivoSaida.close(); //cria o arquivo se nao existir
        return disciplinas; //retorna vetor vazio
    }

    //mesma coisa, repete o processo de leitura linha por linha
    std::string linha;
    while (std::getline(arquivoEntrada, linha)) {
        if (linha.empty()) 
        continue;

        auto campos = _utilidades.dividirLinha(linha, ';');
        if (campos.size() < 5) 
        continue;

        //cria a disciplina com os campos lidos
        int id = std::stoi(campos[0]);
        std::string codigo = campos[1];
        std::string nome = campos[2];
        int professorId = std::stoi(campos[3]);
        int coordDiscId = std::stoi(campos[4]);
        disciplinas.emplace_back(id, codigo, nome, professorId, coordDiscId); //adiciona ao vetor
    }
    arquivoEntrada.close();
    return disciplinas;
}

//salva as disciplinas no arquivo
void GerenciadorDados::salvarDisciplinas(const std::vector<Disciplina>& lista) {

    //abre o arquivo para escrita, apagando o conteudo anterior
    std::ofstream arquivoSaida("data/disciplinas.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir disciplinas.txt\n";
        return;
    }

    //para cada disciplina na lista, salva seus dados e imprime
    for (const auto &d : lista) {
        arquivoSaida << d.getId() << ';' << d.getCodigo() << ';' << d.getNome() << ';' << d.getProfessorId() << ';' << d.getCoordenadorDiscId() << '\n';
    }

    arquivoSaida.close();
}

//carrega as turmas do arquivo
//mesma logica das funcoes anteriores
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

        auto campos = _utilidades.dividirLinha(linha, ';');
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

//salva as turmas no arquivo
void GerenciadorDados::salvarTurmas(const std::vector<Turma>& lista) {
    std::ofstream arquivoSaida("data/turmas.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir turmas.txt\n";
        return;
    }
    //para cada turma na lista, salva seus dados e imprime
    for (const auto &t : lista) {
        arquivoSaida << t.getId() << ';' << t.getDisciplinaId() << ';' << t.getCodigoTurma() << ';' << t.getProfessorId() << '\n';
    }
    arquivoSaida.close();
}


std::vector<Avaliacao> GerenciadorDados::carregarAvaliacoes() {
    std::vector<Avaliacao> avaliacoes;      //vetor para armazenar as avaliacoes
    std::ifstream arquivoEntrada("data/avaliacoes.txt");

    //se o arquivo nao existir, cria um novo
    if (!arquivoEntrada.is_open()) {
        std::ofstream arquivoSaida("data/avaliacoes.txt", std::ios::app);
        arquivoSaida.close();
        return avaliacoes;
    }

    //mesma coisa, repete o processo de leitura linha por linha
    std::string linha;
    while (std::getline(arquivoEntrada, linha)) {
        if (linha.empty()) continue;

        auto campos = _utilidades.dividirLinha(linha, ';');
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


//salva as avaliacoes no arquivo
void GerenciadorDados::salvarAvaliacoes(const std::vector<Avaliacao>& lista) {
    std::ofstream arquivoSaida("data/avaliacoes.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir data/avaliacoes.txt\n";
        return;
    }
    //para cada avaliacao na lista, salva seus dados e imprime
    for (const auto &a : lista) {
        arquivoSaida << a.getId() << ';' << a.getAlvoId() << ';' << a.getTipo() << ';' << a.getNota() << ';' << a.getComentario() << ';'  << _utilidades.getDataAtual() << '\n';
    }
    arquivoSaida.close();
}