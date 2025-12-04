#include <iostream>
#include <string>
#include <limits>

#include "sistemaAvaliacao.hpp"
#include "login.hpp"
#include "usuario.hpp"
#include "senhaMascarada.hpp"


bool feito = false;
bool menuFeito = false;

void menuAluno(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
    menuFeito = false;
    int opc;
    std::string opcStr;
    do {
        std::cout << "\n===== MENU ALUNO =====\n";
        std::cout << "1 - Avaliar disciplina\n";
        std::cout << "2 - Avaliar professor\n";
        std::cout << "3 - Ver Medias de Avaliacao\n";
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        std::cin >> opcStr;
        std::cout << std::endl;

        opc = -1;
        try {opc = std::stoi(opcStr);}
        catch (std::invalid_argument& e) {std::cerr << "ERRO: Entrada deve ser um nu'mero inteiro entre 0 e 3.\n"; continue;}
        catch (std::out_of_range& e) {std::cerr << "ERRO: Entrada deve ser um nu'mero inteiro entre 0 e 3.\n"; continue;}

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
        std::cout << "\n===== MENU PROFESSOR =====\n";
        
        if (isCoordDisc) {
            // Opções completas para Coordenador de Disciplina
            std::cout << "1 - Ver avaliacoes da minha disciplina (Geral)\n"; 
            std::cout << "2 - Ver avaliacoes dos professores (Geral)\n";
            std::cout << "3 - Visualizar minhas Avaliacoes (Filtradas)\n"; // Avaliações relacionadas à sua coordenação (Ponto 1)
            std::cout << "4 - Avaliar turma\n"; 
        } else { 
            // Opções para Professor Comum
            std::cout << "1 - Visualizar minhas Avaliacoes (Filtradas)\n"; // Avaliações dele e da sua disciplina (Ponto 2)
            std::cout << "2 - Avaliar turma\n"; 
        }
        
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        
        if (!(std::cin >> opc)) {
             std::cout << "\nOpcao invalida!\n";
             std::cin.clear(); 
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             continue;
        }

        if (opc == 0) {
            std::cout << "\nSaindo...\n"; 
            feito = true; 
            menuFeito = true; 
            break;
        }

        if (isCoordDisc) {
            switch (opc) {
                case 1: sistema.listarAvaliacoes("DISCIPLINA"); feito = true; menuFeito = false; break;
                case 2: sistema.listarAvaliacoes("PROFESSOR"); feito = true; menuFeito = false; break;
                case 3: sistema.visualizarAvaliacoesProfessor(u); feito = true; menuFeito = false; break;
                case 4: 
                    try {
                        sistema.avaliarTurma(u);
                        feito = true;
                        menuFeito = false;
                    } catch (const char* e) {
                        std::cerr << e;
                    }
                    break;
                default: 
                    std::cout << "\nOpcao invalida!\n";
            }
        } else { // Professor Comum
            switch (opc) {
                case 1: sistema.visualizarAvaliacoesProfessor(u); feito = true; menuFeito = false; break;
                case 2: 
                    try {
                        sistema.avaliarTurma(u);
                        feito = true;
                        menuFeito = false;
                    } catch (const char* e) {
                        std::cerr << e;
                    }
                    break;
                default: 
                    std::cout << "\nOpcao invalida!\n";
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
        std::cout << "1 - Cadastrar usuario (Aluno/Professor)\n"; // Opção 1 agora é só para cadastro
        std::cout << "2 - Cadastrar disciplina\n";
        std::cout << "3 - Cadastrar turma\n";
        std::cout << "4 - Matricular aluno em turma\n"; 
        std::cout << "5 - Ver avaliacoes gerais (Sem detalhes)\n";    
        std::cout << "6 - Relatorio Geral Detalhado (Medias e Comentarios)\n"; 
        std::cout << "0 - Voltar para tela inicial\n";
        std::cout << "Escolha: ";
        std::cin >> opc;
        std::cout << std::endl;

        switch (opc) {
            case 1: { // Cadastrar Usuario
                do {
                    feito = false;
                    std::cout << "\n";
                    int tipo;
                    std::cout << "\nO que deseja fazer?:\n";
                    std::cout << "\n1 - Cadastrar um Aluno\n"; 
                    std::cout << "2 - Cadastrar um Professor\n"; 
                    std::cout << "0 - Voltar ao menu anterior \n";
                    std::cout << "Escolha: ";
                    
                    if (!(std::cin >> tipo)) { 
                        std::cerr << "Caractere invalido. Tente novamente.\n";
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
                        if (tipo == 1) {
                            sistema.cadastrarUsuario(1); // Cadastrar Aluno
                            std::cout << "\nCadastro conclui'do! \n";
                        } else if (tipo == 2) {
                            sistema.cadastrarUsuario(2); // Cadastrar Professor
                            std::cout << "\nCadastro conclui'do! \n";
                        } else {
                            std::cerr << "\nOpcao inva'lida.\n";
                        }
                    }
                    catch (const char* e) {
                        std::cerr << e << std::endl;
                    }
                } while (!feito);
                break;
            }
            case 2: sistema.cadastrarDisciplina();  break;
            case 3: 
                try {sistema.cadastrarTurma(); }
                catch (char const* e) {
                    std::cerr << e << std::endl;
                }
                break;
            case 4: 
                try {sistema.matricularAluno(); }
                catch (char const* e) {
                    std::cerr << e << std::endl;
                }
                break;
            case 5: sistema.listarAvaliacoes("GERAL");  break; 
            case 6: // Relatório Geral Detalhado
                sistema.relatorioGeralCoordenador(); 
                
                break;
            case 0: std::cout << "Saindo...\n"; menuFeito = true; break;
            default: std::cout << "\nOpcao inva'lida!\n";
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
        std::string opcaoStr;
        std::cout << "Escolha uma opcao: \n";
        std::cout << "1 - Fazer login \n";
        std::cout << "0 - Encerrar \n"; 
        std::cout << "Escolha: ";
        std::cin >> opcaoStr;
        std::cout << std::endl;

        try {opcao = std::stoi(opcaoStr);}

        catch (const std::invalid_argument& e) {
            std::cerr << "\nERRO:: Entrada invalida. Digite apenas 0 ou 1.\n";
            continue; // volta para o início do while
        }

        catch (const std::out_of_range& e) {
            std::cerr << "\nERRO:: Numero fora do intervalo. Digite 0 ou 1.\n";
            continue;
        }

        if(opcao == 0){
            std::cout << "\nEncerrando programa... \n";
            break;
        }
        else if(opcao == 1){
            std::cout << "\n===== LOGIN =====\n";
            std::cout << "email: ";
            std::cin >> email;
            
            // LIMPEZA DE BUFFER: Necessário após std::cin >> email para
            // remover o '\n' (Enter) do buffer antes de usar getchar() em lerSenhaMascarada.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            // SUBSTITUIÇÃO: Chama a função mascarada para ler a senha
            senha = lerSenhaMascarada("Senha: ");

            Usuario* u = log.login(sistema.getUsuarios(), email, senha);

            if (u == nullptr) {
                std::cout << "\nCredenciais incorretas! Tente novamente.\n";
                continue;
            }

            std::cout << "\nBem-vindo, " << u->getNome() << " (" << u->getTipo() << ")\n";

            // Redireciona para o menu apropriado
            if (u->getTipo() == "ALUNO") menuAluno(sistema, u);
            else if (u->getTipo() == "PROFESSOR") menuProfessor(sistema, u);
            else if (u->getTipo() == "COORDENADOR_DISCIPLINA") menuProfessor(sistema, u);
            else if (u->getTipo() == "COORDENADOR_DO_CURSO") menuCoordCurso(sistema, u);

            // Salva tudo ao sair do menu
            sistema.salvarTudo();
        }
        else {
            std::cerr << "\nERRO:: Opcao invalida. Escolha 1 - Login ou 0 - Encerrar \n";
        }
    }

    return 0;
}