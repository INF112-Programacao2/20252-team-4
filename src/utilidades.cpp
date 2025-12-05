#include "utilidades.hpp"
#include <iostream>

#include <termios.h> //para manipular configuracoes do terminal
#include <unistd.h> //para STDIN_FILENO, serve para identificar a entrada padrao do terminal
#include <cstdio> //para getchar()

#include <ctime>
#include <iomanip>

#include <sstream>
#include <vector>

//pra preencher Avaliacao::_data no momento do registro
std::string Utilidades::getDataAtual() {
    time_t agora = time(nullptr);
    tm *ltm = localtime(&agora);

    std::ostringstream oss;
    oss << std::put_time(ltm, "%Y-%m-%d");
    return oss.str();
}

std::string Utilidades::lerSenhaMascarada(const std::string& mensagem) {
    struct termios configAntiga, configNova; //estruturas para armazenar configuracoes do terminal
    std::string senha; //variavel para armazenar a senha
    char caractere; //variavel para ler cada caractere

    tcgetattr(STDIN_FILENO, &configAntiga); //pega as configuracoes atuais do terminal
    configNova = configAntiga; //copia para uma nova estrutura

    configNova.c_lflag &= ~(ICANON | ECHO); //desabilita o modo canonico e o echo
    tcsetattr(STDIN_FILENO, TCSANOW, &configNova);//aplica as novas configuracoes

    std::cout << mensagem; 
    std::cout.flush(); //garante que a mensagem seja exibida antes da leitura

    //loop de leitura caractere por caractere
    while ((caractere = getchar()) != '\n' && caractere != EOF) { 
        if (caractere == 127 || caractere == 8) { //lida com Backspace, ASCII 127 ou 8

            if (!senha.empty()) {
                senha.pop_back(); //remove o ultimo caractere da senha
                std::cout << "\b \b"; //move o cursor para tras, imprime um espaco e move de novo para tras
                std::cout.flush(); //garante que a acao seja exibida imediatamente
            }

            } else {
                senha += caractere;
                std::cout << "*"; //exibe o asterisco
                std::cout.flush(); //garante que o asterisco seja exibido imediatamente
            }
    }
    std::cout << "\n"; 
    
    tcsetattr(STDIN_FILENO, TCSANOW, &configAntiga); //restaura as configuracoes antigas do terminal

    return senha;
}


//essa funcao le um inteiro com tratamento de excecoes, garantindo que o valor esteja entre o intervalo permitido
int Utilidades::lerInteiroComExcecao(const std::string& pergunta) {
    std::string entrada;

    while (true) {
        std::cout << pergunta;
        std::getline(std::cin >> std::ws, entrada);

        try {
            int valor = std::stoi(entrada); //converte a string para inteiro

            // Verifica intervalo permitido
            if (valor < 0 || valor > 5) {
                std::cout << "Erro: o valor deve estar entre 0 e 5.\n";
                continue; // volta ao inicio do loop
            }

            return valor; // valor valido
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Erro: digite apenas números inteiros.\n";
        }
    }
}

//funcao auxiliar para dividir uma linha em partes com base em um delimitador
std::vector<std::string> Utilidades::dividirLinha(const std::string &linha, char delim) {
    std::vector<std::string> partes; //vetor para armazenar as partes
    std::stringstream fluxo(linha); //cria um fluxo de string a partir da linha
    std::string campo; //variavel para armazenar cada campo
    while (std::getline(fluxo, campo, delim)) partes.push_back(campo); //le cada campo e adiciona ao vetor
    return partes; //retorna o vetor de partes
}