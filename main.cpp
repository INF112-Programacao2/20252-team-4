#include <iostream>
#include <string>
#include <limits>

#include "sistemaAvaliacao.hpp"
#include "login.hpp"
#include "usuario.hpp"

using namespace std;

// ===== FUNÇÕES AUXILIARES DE MENUS =====

bool feito = false;
bool menuFeito = false;
void menuAluno(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
    menuFeito = false;
    int opc;
    do {
        cout << "\n===== MENU ALUNO =====\n";
        cout << "1 - Avaliar disciplina\n";
        cout << "2 - Avaliar professor\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opc;

        switch (opc) {
            case 1: 
                try {sistema.avaliarDisciplina(u); feito = true; menuFeito = false;}
                catch (const char* e) {
                    std::cerr << e;
                }
                break;

            case 2: 
                try {sistema.avaliarProfessor(u); feito = true; menuFeito = false;}
                catch (const char* e) {
                    std::cerr << e;
                }
                break;
            case 0: cout << "Saindo...\n"; feito = true; menuFeito = true; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (!menuFeito);
}

void menuProfessor(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
    menuFeito = false;
    int opc;
    do {
        cout << "\n===== MENU PROFESSOR =====\n";
        cout << "1 - Ver avaliacoes da minha disciplina\n";
        cout << "2 - Ver avaliacoes das minhas turmas\n";
        cout << "3 - Avaliar turma\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opc;

        switch (opc) {
            case 1: sistema.listarAvaliacoes("DISCIPLINA"); feito = true; menuFeito = false; break;
            case 2: sistema.listarAvaliacoes("TURMA"); feito = true; menuFeito = false; break;
            case 3: 
                try {
                    sistema.avaliarTurma(u);
                    feito = true;
                    menuFeito = false;
                }
                catch (const char* e) {
                    std::cerr << e;
                }
                break;
            case 0: cout << "Saindo...\n"; feito = true; menuFeito = true; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (!menuFeito);
}

void menuCoordDisciplina(SistemaAvaliacao &sistema, Usuario* u) {

    // FAZER TRATAMENTO DE EXCECAO AQUI !!!!!!!!!!!

    feito = false;
    int opc;
    do {
        cout << "\n===== MENU COORDENADOR DE DISCIPLINA =====\n";
        cout << "1 - Ver avaliacoes das minhas disciplinas\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opc;

        switch (opc) {
            case 1: sistema.listarAvaliacoes("DISCIPLINA"); break;
            case 0: cout << "Saindo...\n"; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (opc != 0);
}

void menuCoordCurso(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
    int opc;
    do {
        cout << "\n===== MENU COORDENADOR DE CURSO (ADMIN) =====\n";
        cout << "1 - Cadastrar usuario\n";
        cout << "2 - Cadastrar disciplina\n";
        cout << "3 - Cadastrar turma\n";
        cout << "4 - Ver avaliacoes gerais\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opc;
        switch (opc) {
            case 1: {
                do {
                    std::cout << "\n";
                    int tipo;
                    std::cout << "Escolha o tipo de usuario:\n";
                    std::cout << "1 - Aluno\n";
                    std::cout << "2 - Professor\n";
                    std::cout << "Escolha: ";
                    
                    if (!(std::cin >> tipo)) { // Verifica se a leitura falhou
                        std::cerr << "Caractere invalido. Escolha 1 (Aluno) ou 2 (Professor).\n";
                        
                        // 1. Limpa o estado de erro
                        std::cin.clear(); 
                        
                        // 2. Descarta o buffer (MAX_SIZE é um valor grande para garantir que tudo seja lido, \n é o delimitador)
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        
                        feito = false; // Repete o loop
                        continue; // Vai para a próxima iteração do 'do-while'
                    }

                    try {
                        sistema.cadastrarUsuario(tipo);
                        feito = true;
                        menuFeito = false;
                    }
                    catch (const char* e) {
                        std::cerr << e << std::endl;
                        feito = false; // Repete o loop se o tipo for (ex: 3)
                    }
                } while (!feito);
                break;
            }
            case 2: sistema.cadastrarDisciplina(); feito = true; menuFeito = false; break;
            case 3: 
                try {sistema.cadastrarTurma(); feito = true; menuFeito = false;}
                catch (char const* e) {
                    std::cerr << e << std::endl;
                }
                break;
            case 4: sistema.listarAvaliacoes("GERAL"); feito = true; menuFeito = false; break;
            case 0: cout << "Saindo...\n"; feito = true; menuFeito = true; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (!menuFeito);
}


// =================== MAIN ===================

int main() {
    SistemaAvaliacao sistema;
    Login log;

    while (true) {
        string email, senha;

        cout << "\n===== SISTEMA DE AVALIACAO =====\n";

        int opcao;
        std::cout << "Escolha uma opcao: \n";
        std::cout << "1 - fazer login \n";
        std::cout << "0 - Encerrar \n"; 
        std::cout << "Escolha: ";
        std::cin >> opcao;

        if(opcao == 0){
            std::cout << "Encerrando programa... \n";
            break;
        }
        else if(opcao == 1){
            cout << "\n===== LOGIN =====\n";
            cout << "email: ";
            cin >> email;
            cout << "Senha: ";
            cin >> senha;

            Usuario* u = log.login(sistema.getUsuarios(), email, senha);

            if (u == nullptr) {
                cout << "Credenciais incorretas! Tente novamente.\n";
                continue;
            }

            cout << "\nBem-vindo, " << u->getNome() << " (" << u->getTipo() << ")\n";

            // Redireciona para o menu apropriado
            if (u->getTipo() == "ALUNO") menuAluno(sistema, u);
            else if (u->getTipo() == "PROFESSOR") menuProfessor(sistema, u);
            else if (u->getTipo() == "COORDENADOR_DISCIPLINA") menuCoordDisciplina(sistema, u);
            else if (u->getTipo() == "COORDENADOR_DO_CURSO") menuCoordCurso(sistema, u);

            // Salva tudo ao sair do menu
            sistema.salvarTudo();
        }
        else {
            std::cerr << "ERRO:: Opcao invalida. Escolha 1 - Login ou 0 - Encerrar \n";
        }
    }

    return 0;
}