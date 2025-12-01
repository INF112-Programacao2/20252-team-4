#include <iostream>
#include <string>

#include "sistemaAvaliacao.hpp"
#include "login.hpp"
#include "usuario.hpp"

using namespace std;

// ===== FUNÇÕES AUXILIARES DE MENUS =====

bool feito = false;
void menuAluno(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
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
                try {sistema.avaliarDisciplina(u); feito = true;}
                catch (const char* e) {
                    std::cerr << e;
                }
                break;

            case 2: 
                try {sistema.avaliarProfessor(u); feito = true;}
                catch (const char* e) {
                    std::cerr << e;
                }
                break;
            case 0: cout << "Saindo...\n"; feito = true; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (!feito);
}

void menuProfessor(SistemaAvaliacao &sistema, Usuario* u) {
    feito = false;
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
            case 1: sistema.listarAvaliacoes("DISCIPLINA"); feito = true; break;
            case 2: sistema.listarAvaliacoes("TURMA"); feito = true; break;
            case 3: 
                try {
                    sistema.avaliarTurma(u);
                    feito = true;
                }
                catch (const char* e) {
                    std::cerr << e;
                }
                break;
            case 0: cout << "Saindo...\n"; feito = true; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (!feito);
}

void menuCoordDisciplina(SistemaAvaliacao &sistema, Usuario* u) {
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
        std::cout << "\n";
        switch (opc) {
            case 1: {
                do {
                int tipo;
                    cout << "Escolha o tipo de usuario:\n";
                    cout << "1 - Aluno\n";
                    cout << "2 - Professor\n";
                    cin >> tipo;
                    try {
                        sistema.cadastrarUsuario(tipo);
                        feito = true;
                    }
                    catch (const char* e) {
                        std::cerr << e << std::endl;
                        feito = false;
                    }
                } while (!feito);
                break;
            }
            case 2: sistema.cadastrarDisciplina(); feito = true; break;
            case 3: 
                try {sistema.cadastrarTurma(); feito = true;}
                catch (char const* e) {
                    std::cerr << e << std::endl;
                }
                break;
            case 4: sistema.listarAvaliacoes("GERAL"); feito = true; break;
            case 0: cout << "Saindo...\n"; feito = true; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (!feito);
}


// =================== MAIN ===================

int main() {
    SistemaAvaliacao sistema;
    Login log;

    cout << "===== SISTEMA DE AVALIACAO =====\n";

    while (true) {
        string matricula, senha;

        cout << "\nLogin\n";
        cout << "matricula: ";
        cin >> matricula;
        cout << "Senha: ";
        cin >> senha;

        Usuario* u = log.login(sistema.getUsuarios(), matricula, senha);

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

    return 0;
}