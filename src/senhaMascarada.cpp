#include "senhaMascarada.hpp"
#include <string>
#include <iostream>
#include <termios.h> //para manipular configuracoes do terminal
#include <unistd.h>  //para STDIN_FILENO, serve para identificar a entrada padrao do terminal
#include <cstdio> //para getchar()

std::string lerSenhaMascarada(const std::string& mensagem) {
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