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
        std::cout << "3 - Ver Medias de Avaliacao\n"; // NOVA OPÇÃO
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
            case 3: // NOVA OPÇÃO
                sistema.visualizarMediasAluno(u);
                feito = true; menuFeito = false;
                break;
            case 0: std::cout << "Saindo...\n"; feito = true; menuFeito = true; break;
            default: std::cout << "Opcao invalida!\n";
        }
    } while (!menuFeito);
}

void menuProfessor(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
    menuFeito = false;
    std::string tipo = u->getTipo();
    bool isCoordDisc = (tipo == "COORDENADOR_DISCIPLINA");
    int opc;
    do {
        std::cout << "\n===== MENU PROFESSOR (" << tipo << ") =====\n";
        
        // Professor Coordenador de Disciplina (Ponto 1)
        if (isCoordDisc) {
            std::cout << "1 - Ver avaliacoes da minha disciplina (Geral)\n"; 
            std::cout << "2 - Ver avaliacoes dos professores (Geral)\n";
        }
        
        // Opções para Professor Comum (Ponto 2)
        int opc_visualizar = isCoordDisc ? 3 : 1;
        int opc_avaliar_turma = isCoordDisc ? 4 : 2;

        std::cout << opc_visualizar << " - Visualizar minhas Avaliacoes (Filtradas)\n"; 
        std::cout << opc_avaliar_turma << " - Avaliar turma\n"; 
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        
        if (!(std::cin >> opc)) { // Leitura segura para evitar problemas no loop
             std::cout << "Opcao invalida!\n";
             std::cin.clear(); 
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             continue; // Recomeça o loop
        }

        if (opc == 0) {
            std::cout << "Saindo...\n"; 
            feito = true; 
            menuFeito = true; 
            break;
        }

        if (isCoordDisc) {
            switch (opc) {
                case 1: 
                case 2: 
                    sistema.listarAvaliacoes(opc == 1 ? "DISCIPLINA" : "PROFESSOR"); 
                    feito = true; 
                    menuFeito = false; 
                    break;
                case 3: 
                    if (opc == opc_visualizar) {
                         sistema.visualizarAvaliacoesProfessor(u); // FILTRAGEM AVANÇADA
                         feito = true; 
                         menuFeito = false; 
                    } else {
                        std::cout << "Opcao invalida!\n";
                    }
                    break;
                case 4: 
                    if (opc == opc_avaliar_turma) {
                        try {
                            sistema.avaliarTurma(u);
                            feito = true;
                            menuFeito = false;
                        }
                        catch (const char* e) {
                            std::cerr << e;
                        }
                    } else {
                        std::cout << "Opcao invalida!\n";
                    }
                    break;
                default: 
                    std::cout << "Opcao invalida!\n";
            }
        } else { // Professor Comum (Ponto 2)
            switch (opc) {
                case 1: // Visualizar minhas Avaliacoes (Filtradas)
                    if (opc == opc_visualizar) {
                        sistema.visualizarAvaliacoesProfessor(u); // FILTRAGEM AVANÇADA
                        feito = true; 
                        menuFeito = false; 
                    } else {
                        std::cout << "Opcao invalida!\n";
                    }
                    break;
                case 2: // Avaliar turma
                    if (opc == opc_avaliar_turma) {
                        try {
                            sistema.avaliarTurma(u);
                            feito = true;
                            menuFeito = false;
                        }
                        catch (const char* e) {
                            std::cerr << e;
                        }
                    } else {
                         std::cout << "Opcao invalida!\n";
                    }
                    break;
                default: 
                    std::cout << "Opcao invalida!\n";
            }
        }

        if (!menuFeito) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        
    } while (!menuFeito);
}

void menuCoordCurso(SistemaAvaliacao &sistema, Usuario* u) {
    menuFeito = false;
    feito = false;
    int opc;
    do {
        std::cout << "\n===== MENU COORDENADOR DE CURSO (ADMIN) =====\n";
        std::cout << "1 - Cadastrar usuario (Aluno/Professor)\n"; 
        std::cout << "2 - Cadastrar disciplina\n";
        std::cout << "3 - Cadastrar turma\n";
        std::cout << "4 - Matricular aluno em turma\n"; 
        std::cout << "5 - Ver avaliacoes gerais (Sem detalhes)\n";    
        std::cout << "6 - Relatorio Geral Detalhado (Medias e Comentarios)\n"; // NOVA OPÇÃO (Ponto 4)
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        std::cin >> opc;
        switch (opc) {
            case 1: { // Cadastrar Usuario
                do {
                    feito = false;
                    std::cout << "\n";
                    int tipo;
                    std::cout << "O que deseja fazer?:\n";
                    std::cout << "\n1 - Cadastrar um Aluno\n";
                    std::cout << "2 - Cadastrar um Professor\n";
                    std::cout << "0 - Voltar ao menu anterior \n";
                    std::cout << "Escolha: ";
                    
                    if (!(std::cin >> tipo)) { 
                        std::cerr << "Caractere invalido. Escolha 1 (Aluno) ou 2 (Professor).\n";
                        std::cin.clear(); 
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        continue; 
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
            case 2: sistema.cadastrarDisciplina(); break;
            case 3: 
                try {sistema.cadastrarTurma();}
                catch (char const* e) {
                    std::cerr << e << std::endl;
                }
                break;
            case 4: 
                try {sistema.matricularAluno();}
                catch (char const* e) {
                    std::cerr << e << std::endl;
                }
                break;
            case 5: sistema.listarAvaliacoes("GERAL"); break; 
            case 6: // Relatório Geral Detalhado (Ponto 4)
                sistema.relatorioGeralCoordenador(); 
                break;
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