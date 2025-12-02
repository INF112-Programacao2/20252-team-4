#include <iostream>
#include <string>
#include <limits>

#include "sistemaAvaliacao.hpp"
#include "login.hpp"
#include "usuario.hpp"


bool feito = false;
bool menuFeito = false;
void menuAluno(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
    menuFeito = false;
    int opc;
    do {
        std::cout << "\n===== MENU ALUNO =====\n";
        std::cout << "1 - Avaliar disciplina\n";
        std::cout << "2 - Avaliar professor\n";
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        std::cin >> opc;

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
            case 0: std::cout << "Saindo...\n"; feito = true; menuFeito = true; break;
            default: std::cout << "Opcao invalida!\n";
        }
    } while (!menuFeito);
}

void menuProfessor(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
    menuFeito = false;
    int opc;
    do {
        std::cout << "\n===== MENU PROFESSOR =====\n";
        std::cout << "1 - Ver avaliacoes da minha disciplina\n";
        std::cout << "2 - Ver avaliacoes das minhas turmas\n";
        std::cout << "3 - Avaliar turma\n";
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        std::cin >> opc;

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
            case 0: std::cout << "Saindo...\n"; feito = true; menuFeito = true; break;
            default: std::cout << "Opcao invalida!\n";
        }
    } while (!menuFeito);
}

void menuCoordDisciplina(SistemaAvaliacao &sistema, Usuario* u) {

    // FAZER TRATAMENTO DE EXCECAO AQUI !!!!!!!!!!!

    feito = false;
    int opc;
    do {
        std::cout << "\n===== MENU COORDENADOR DE DISCIPLINA =====\n";
        std::cout << "1 - Ver avaliacoes da minha disciplina\n";
        std::cout << "2 - Ver avaliacoes dos professores da disciplina\n";
        std::cout << "3 - Ver avaliacoes das turmas\n";
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        std::cin >> opc;

        switch (opc) {
            case 1: sistema.listarAvaliacoes("DISCIPLINA"); break;
            case 0: std::cout << "Saindo...\n"; break;
            default: std::cout << "Opcao invalida!\n";
        }
    } while (opc != 0);
}

void menuCoordCurso(SistemaAvaliacao &sistema, Usuario* u) {
    menuFeito = false;
    feito = false;
    int opc;
    do {
        std::cout << "\n===== MENU COORDENADOR DE CURSO (ADMIN) =====\n";
        std::cout << "1 - Cadastrar usuario (Aluno/Professor)\n"; // Manter o cadastro em um sub-menu
        std::cout << "2 - Cadastrar disciplina\n";
        std::cout << "3 - Cadastrar turma\n";
        std::cout << "4 - Matricular aluno em turma\n"; // NOVA OPÇÃO PRINCIPAL
        std::cout << "5 - Ver avaliacoes gerais\n";    // Opção 4 anterior
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        std::cin >> opc;
        switch (opc) {
            case 1: { // Cadastrar Usuario (sub-menu mantido para escolha de tipo)
                do {
                    feito = false;
                    std::cout << "\n";
                    int tipo;
                    std::cout << "O que deseja fazer?:\n";
                    std::cout << "\n1 - Cadastrar um Aluno\n";
                    std::cout << "2 - Cadastrar um Professor\n";
                    std::cout << "0 - Voltar ao menu anterior \n";
                    std::cout << "Escolha: ";
                    
                    if (!(std::cin >> tipo)) { // Verifica se a leitura falhou
                        std::cerr << "Caractere invalido. Escolha 1 (Aluno) ou 2 (Professor).\n";
                        
                        // 1. Limpa o estado de erro
                        std::cin.clear(); 
                        
                        // 2. Descarta o buffer 
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        
                        continue; // Vai para a próxima iteração do 'do-while'
                    }
 
                    std::cout << std::endl;

                    if(tipo == 0){
                        feito = true;
                        continue;
                    }

                    try {
                        sistema.cadastrarUsuario(tipo);
                        std::cout << "\nCadastro concluido! \n";
                    }
                    catch (const char* e) {
                        std::cerr << e << std::endl;
                    }
                } while (!feito);
                break;
            }
            case 2: sistema.cadastrarDisciplina(); menuFeito = true; break;
            case 3: 
                try {sistema.cadastrarTurma(); menuFeito = true;}
                catch (char const* e) {
                    std::cerr << e << std::endl;
                }
                break;
            case 4: // Matricular Aluno em Turma (Nova posição)
                try {sistema.matricularAluno(); menuFeito = true;}
                catch (char const* e) {
                    std::cerr << e << std::endl;
                }
                break;
            case 5: sistema.listarAvaliacoes("GERAL"); menuFeito = true; break; // Antiga opção 4
            case 0: std::cout << "Saindo...\n"; menuFeito = true; break;
            default: std::cout << "Opcao invalida!\n";
        }
    } while (!menuFeito);
}


int main() {
    SistemaAvaliacao sistema;
    Login log;

    while (true) {
        std::string email, senha;

        std::cout << "\n===== SISTEMA DE AVALIACAO DO DPI =====\n";

        int opcao;
        std::cout << "Escolha uma opcao: \n";
        std::cout << "1 - fazer login \n";
        std::cout << "0 - Encerrar \n"; 
        std::cout << "Escolha: ";
        std::cin >> opcao;
        std::cout << std::endl;

        if(opcao == 0){
            std::cout << "Encerrando programa... \n";
            break;
        }
        else if(opcao == 1){
            std::cout << "\n===== LOGIN =====\n";
            std::cout << "email: ";
            std::cin >> email;
            std::cout << "Senha: ";
            std::cin >> senha;

            Usuario* u = log.login(sistema.getUsuarios(), email, senha);

            if (u == nullptr) {
                std::cout << "Credenciais incorretas! Tente novamente.\n";
                continue;
            }

            std::cout << "\nBem-vindo, " << u->getNome() << " (" << u->getTipo() << ")\n";

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