#include "getDataAtual.hpp"

std::string getDataAtual() {
    time_t agora = time(nullptr);
    tm *ltm = localtime(&agora);

    std::ostringstream oss;
    oss << std::put_time(ltm, "%Y-%m-%d");
    return oss.str();
}