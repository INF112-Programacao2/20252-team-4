#include "senhaMascarada.hpp"
#include <string>
#include <iostream>
#include <termios.h> 
#include <unistd.h>  
#include <cstdio> // Para getchar()

std::string lerSenhaMascarada(const std::string& mensagem) {
    // VARIAVEIS PARA CONFIGURAÇÃO DO TERMINAL
    struct termios configAntiga, configNova; 
    std::string senha;
    char caractere; 

  
    tcgetattr(STDIN_FILENO, &configAntiga);
    configNova = configAntiga;

 
    configNova.c_lflag &= ~(ICANON | ECHO);
    // Aplica as novas configuracoes imediatamente
    tcsetattr(STDIN_FILENO, TCSANOW, &configNova);

    std::cout << mensagem; 
    std::cout.flush(); // Garante que a mensagem seja exibida antes da leitura

    // Loop de leitura caractere por caractere
    while ((caractere = getchar()) != '\n' && caractere != EOF) { 
        if (caractere == 127 || caractere == 8) { // Lida com Backspace
            if (!senha.empty()) {
                senha.pop_back();
                // Imprime \b (backspace), espaço em branco (apaga o *), e \b (volta o cursor)
                std::cout << "\b \b";
                std::cout.flush();
            }
        } else {
            senha += caractere;
            std::cout << "*"; // Exibe o asterisco
            std::cout.flush();
        }
    }
    std::cout << "\n"; 

    
    tcsetattr(STDIN_FILENO, TCSANOW, &configAntiga);

    return senha;
}