#ifndef SENHAMASCARADA_HPP
#define SENHAMASCARADA_HPP

#include <string>
#include <iostream>
#include <termios.h> // Para sistemas Unix-like (Linux/macOS)
#include <unistd.h>  // Para STDIN_FILENO


std::string lerSenhaMascarada(const std::string& mensagem);

#endif // SENHAMASCARADA_HPP