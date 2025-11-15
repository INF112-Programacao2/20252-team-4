#include "gerenciadordados.hpp"
#include <iostream>
#include <stdexcept>

//Construtor
GerenciadorDados::GerenciadorDados() {
    try {
        //Carrega todos os dados do arquivo .tvt 
        carregar_usuarios();
        carregar_disciplinas();
        carregar_turmas();
        carregar_avaliacoes(); 
    } catch (const std::exception& e) {
        //Reporta o erro se ouver falha ao carreagar os dados.
        std::cerr << "AVISO: Falha ao carregar alguns dados. Iniciando com dados vazios." << std::endl;
    }
}

//Destrutor
GerenciadorDados::~GerenciadorDados() {
    try {
        //Salva todos  os dados modificados no arquivo
        salvar_usuarios();
        salvar_disciplinas();
        salvar_turmas();
        salvar_avaliacoes();
    } catch (const std::exception& e) {
        //Reporta o erro casso o os dados nao seja salvos
        std::cerr << "ERRO: Falha ao salvar os dados. Algumas alteracoes podem ter sido perdidas." << std::endl;
    }
}

void GerenciadorDados::salvar_avaliacoes() const {
    //Abre o arquivo para a escrita
    std::ofstream arquivo(ARQ_AVALIACOES);
    
    //Verifica se o arquivo abriu 
    if (arquivo.is_open()) {
        //Itera sobre todos os objeto da Avaliacao na memoria para salvar
        for (const auto& avaliacao : _avaliacoes) {
            //Vou ainda implementar os objetos da classe para texto para salva no .txt 
             
        }
        //Fecha a conexao do .txt
        arquivo.close();
    } else {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo " << ARQ_AVALIACOES << " para salvar." << std::endl;
    }
}

void GerenciadorDados::carregar_avaliacoes() {
     //Abre o arquivo para a leitura
    std::ifstream arquivo(ARQ_AVALIACOES);
    //linpa a lista na memoria antes de carregar-la para que nao haja mistura de dados 
    _avaliacoes.clear(); 

      
    //Verifica se o arquivo abriu 
    if (arquivo.is_open()) {
        //loop para que a leitura do arquivo nao pare ate chegar no fim
        while (!arquivo.eof()) {
            try {//terei ainda de implementar o codigo de leitura de acordo com modificacao da classe 
                
                
            } catch (const std::exception& e) {
                // Se der erro ao ler um dado
                break; 
            }
        }
         //Fecha a conexao do .txt
        arquivo.close();
    } 
}

void GerenciadorDados::salvar_usuarios() const {
    //Abre o arquivo para a escrita
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
    //Abre o arquivo para a escrita
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
    // Adiciona uma nova Avaliacao ao final da lista de avaliacoes na memoria.
    _avaliacoes.push_back(avaliacao);
}

const std::vector<Avaliacao>& GerenciadorDados::get_avaliacoes() const {
    // Devolve a lista completa de avaliacoes. O 'const' garante que ninguem vai mudar a lista.
    return _avaliacoes;
}

void GerenciadorDados::adicionar_usuario(const Usuario& usuario) {
    // Adiciona um novo Usuario ao final da lista de usuarios na memoria.
    _usuarios.push_back(usuario);
}

const std::vector<Usuario>& GerenciadorDados::get_usuarios() const {
    // Devolve a lista completa de usuarios.
    return _usuarios;
}

Usuario* GerenciadorDados::buscar_usuario_por_ID(int ID) {
    // Busca um usuario na lista usando o numero de identificacao (ID).
    for (auto& usuario : _usuarios) {
        if (usuario.get_ID() == ID) {
            // Se achar, devolve o endereco do usuario (ponteiro).
            return &usuario; 
        }
    }
    // Se nao achar ninguem com esse ID, devolve 'nullptr' (nada).
    return nullptr; 
}

void GerenciadorDados::adicionar_disciplina(const Disciplina& disciplina) {
    // Adiciona uma nova Disciplina ao final da lista na memoria.
    _disciplinas.push_back(disciplina);
}

const std::vector<Disciplina>& GerenciadorDados::get_disciplinas() const {
    // Devolve a lista completa de disciplinas.
    return _disciplinas;
}

void GerenciadorDados::adicionar_turma(const Turma& turma) {
    // Adiciona uma nova Turma ao final da lista na memoria.
    _turmas.push_back(turma);
}

const std::vector<Turma>& GerenciadorDados::get_turmas() const {
    // Devolve a lista completa de turmas.
    return _turmas;
}