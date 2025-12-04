#include "sistemaAvaliacao.hpp"
#include <iostream>
#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>
#include <filesystem>

//essa funcao le um inteiro com tratamento de excecoes, garantindo que o valor esteja entre o intervalo permitido
int lerInteiroComExcecao(const std::string& pergunta) {
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
        catch (const std::invalid_argument&) {
            std::cout << "Erro: digite apenas números inteiros.\n";
        }
    }
}

// carrega tudo
SistemaAvaliacao::SistemaAvaliacao() {

    // carrega os dados do GerenciadorDados
    arquivo.carregarUsuarios(_usuarios);
    _disciplinas = arquivo.carregarDisciplinas();
    _turmas = arquivo.carregarTurmas();
    _avaliacoes = arquivo.carregarAvaliacoes();
    arquivo.carregarMatriculas(_usuarios, _turmas);

    // Se nao existir nenhum coordenador do curso, cria um padrao
    bool temCoord = false;
    for (auto u : _usuarios) if (u->getTipo() == "COORDENADOR_DO_CURSO") { 
        temCoord = true; 
        break; 
    }
    if (!temCoord) {
        // adiciona um administrador padrao
        int proximoId = ProximoIdUsuarios(_usuarios);
        CoordenadorCurso *admin = new CoordenadorCurso(proximoId, "Salles", "admin@ufv.br", "admin123");
        _usuarios.insert(_usuarios.begin(), admin);
    }
}

// procura a disciplina por ID
Disciplina* SistemaAvaliacao::getDisciplinaPorID(int id) {
    for (auto &d : _disciplinas) {
        if (d.getId() == id) return &d;
    }
    return nullptr;
}

//gets ----------------------------------------------------------
Turma* SistemaAvaliacao::getTurmaPorID(int id) {
    for (auto &t : _turmas) {
        if (t.getId() == id) return &t;
    }
    return nullptr;
}

Usuario* SistemaAvaliacao::getUsuarioPorID(int id) {
    for (auto *u : _usuarios) {
        if (u->getId() == id) return u;
    }
    return nullptr;
}

std::vector<Usuario*>& SistemaAvaliacao::getUsuarios() {
    return _usuarios;
}
//---------------------------------------------------------------

// libera ponteiros de _usuarios
SistemaAvaliacao::~SistemaAvaliacao() {
    for (auto u : _usuarios) 
    delete u; //libera memoria
}

// gerar novo id para cada tipo -----------------------------
int SistemaAvaliacao::ProximoIdUsuarios(const std::vector<Usuario*> &v) {
    
    int ultimoId = 0; //armazenar o ultimo id encontrado, inicia em 0
    for (auto u : v) 
        if (u->getId() > ultimoId) ultimoId = u->getId();
            return ultimoId + 1; //retorna o proximo id disponivel
}

int SistemaAvaliacao::ProximoIdDisciplinas(const std::vector<Disciplina> &v) {
    int ultimoId = 0; //armazenar o ultimo id encontrado, inicia em 0
    for (auto &d : v) 
        if (d.getId() > ultimoId) ultimoId = d.getId();
    return ultimoId + 1; //retorna o proximo id disponivel
}

int SistemaAvaliacao::ProximoIdTurmas(const std::vector<Turma> &v) {
    int ultimoId = 0;
    for (auto &t : v) 
        if (t.getId() > ultimoId) ultimoId = t.getId();
            return ultimoId + 1; //retorna o proximo id disponivel
}

int SistemaAvaliacao::ProximoIdAvaliacoes(const std::vector<Avaliacao> &v) {
    int ultimoId = 0;
    for (auto &a : v) 
        if (a.getId() > ultimoId) ultimoId = a.getId();
            return ultimoId + 1; //retorna o proximo id disponivel
}

//----------------------------------------------------------------------

// CADASTROS-----------------------------------------------------------
void SistemaAvaliacao::cadastrarUsuario(const int &tipo) {

    //verifica se o tipo e valido
    if (tipo != 1 && tipo != 2) {
        throw "\nTipo de usuario invalido. Escolha 1 (Aluno) ou 2 (Professor).\n";
    }

    std::string nome, email, senha; //variaveis para armazenar os dados do usuario

    std::cout << "Nome: ";
    std::getline(std::cin >> std::ws, nome); //usamos std::ws para descartar espaco em branco, quebra de linha e tabulacao

    const std::string validarEmail = "@ufv.br"; //dominio obrigatorio do email, precisa terminar em @ufv.br
    const size_t tamanhoEmail = validarEmail.length(); //tamanho do dominio

    while (true) {

        std::cout << "email: ";
        
        if (!(std::cin >> email)) {
            std::cerr << "\nErro na leitura do email.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; //volta para o inicio do loop
        }

        //Verificar formato do email
        if (email.length() < tamanhoEmail || 
            email.substr(email.length() - tamanhoEmail) != validarEmail ||
            email.length() == tamanhoEmail) {
            std::cerr << "\nERRO: O e-mail deve ser do dominio '@ufv.br'. Tente novamente.\n";
            continue; //se o formato for invalido, volta para o inicio do loop
        }

        //Verificar se email e' repetido
        bool emailExiste = false;
        for (const auto* u : _usuarios) {
            if (u->getemail() == email) {
                emailExiste = true;
                break; //se encontrar, marca como existente e sai do loop
            }
        }

        if (emailExiste) {
            std::cerr << "\nERRO: Usuario " << email << " ja esta cadastrado no sistema. Tente novamente.\n";
            continue; //se o email ja existir, volta para o inicio do loop
        }
        break;
    }

    std::cout << "Senha: ";
    std::cin >> senha;

    int id = ProximoIdUsuarios(_usuarios);
    Usuario* novo = nullptr;
    if (tipo == 1) novo = new Aluno(id, nome, email, senha);
    else if (tipo == 2) novo = new Professor(id, nome, email, senha);

    _usuarios.push_back(novo);
    std::cout << "Usuario cadastrado com ID: " << id << "\n";

    arquivo.salvarUsuarios(_usuarios);
}

//cadastra uma nova disciplina
void SistemaAvaliacao::cadastrarDisciplina() {

    std::string codigo, nome;
    int coordenadorId;
    
    //tratamento de excecao para o codigo da disciplina
    while (true) {
        std::cout << "Codigo da disciplina (ex: INF112): ";
        
        if (!(std::cin >> codigo)) {
            std::cerr << "\nErro na leitura do codigo.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            continue; //volta para o inicio do loop
        }

        if (codigo.length() != 6 || codigo.substr(0, 3) != "INF") {
            std::cerr << "\nERRO: O codigo da disciplina deve ter 6 caracteres, começando com 'INF' (em maiusculas) e seguido por tres numeros (ex.: INF112). Tente novamente.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; //volta para o inicio do loop
        }
        
        int numCodigo;//variavel para armazenar os numeros do codigo

        try {

            std::string numStr = codigo.substr(3); //pega os tres caracteres apos 'INF'
            numCodigo = std::stoi(numStr); //converte para inteiro

            if (numCodigo < 100 || numCodigo > 999) { //verifica se esta entre 100 e 999
                std::cerr << "\nERRO: Os tres numeros do codigo (depois de 'INF') devem estar entre 100 e 999. Tente novamente.\n";
                continue; //volta para o inicio do loop
            }
        }
        catch (const std::invalid_argument&) { //se nao for possivel converter
            std::cerr << "\nERRO: Os caracteres apos 'INF' devem ser numeros validos. Tente novamente.\n";
            continue; //volta para o inicio do loop
        } 
        catch (const std::out_of_range&) {
            std::cerr << "\nERRO: Numero fora do intervalo. Tente novamente.\n";
            continue; //volta para o inicio do loop
        }

        bool codigoExiste = false; //verifica se o codigo ja existe
        for (const auto &d : _disciplinas) {
            if (d.getCodigo() == codigo) {
                codigoExiste = true;
                break; //se encontrar, marca como existente e sai do loop
            }
        }
        //verifica se o codigo ja existe
        if (codigoExiste) {
            std::cerr << "\nERRO: Codigo de disciplina '" << codigo << "' ja' cadastrado. Tente outro.\n";
            continue;  //volta para o inicio do loop
        } else {
            break; 
        }
    }
    
    //Tratamento de excecao para o nome de disciplina
    while (true) {
        std::cout << "\nNome da disciplina: ";
        std::getline(std::cin >> std::ws, nome);
        //verifica se o nome e vazio
        if (nome.empty()) {
            std::cerr << "\nERRO: O nome da disciplina nao pode ser vazio. Tente novamente.\n";
            continue;
        }
        
        bool nomeExiste = false; //verifica se o nome ja existe
        //verifica se o nome ja existe
        for (const auto &d : _disciplinas) {
            if (d.getNome() == nome) {
                nomeExiste = true;
                break;
            }
        }
        //se o nome ja existir, volta para o inicio do loop
        if (nomeExiste) {
            std::cerr << "\nERRO: Nome de disciplina '" << nome << "' ja' cadastrado. Tente outro.\n";
            continue;
        } else {
            break; //nome valido, sai do loop
        }
    }

  std::cout << "\nProfessores disponiveis para coordenador:\n";

  std::vector<Usuario*> professoresDisponiveis; //vetor para armazenar os professores disponiveis

    for (auto u : _usuarios) {
        //mostra apenas professores e coordenadores de disciplina
        std::string tipo = u->getTipo();

        //permite coordenadores de curso, disciplina ou professor comum como coordenador
        if (tipo == "PROFESSOR" || tipo == "COORDENADOR_DISCIPLINA" || tipo == "COORDENADOR_DO_CURSO") {
            std::cout << u->getId() << " - " << u->getNome() << " (" << tipo << ")\n";
            professoresDisponiveis.push_back(u); //adiciona ao vetor
        }
    }
    //tratamento de excecao para o id do coordenador
    Usuario* profCoordenador = nullptr;

    while(profCoordenador == nullptr) { //enquanto nao encontrar um coordenador valido

        std::cout << "\nEscolha o ID do professor para COORDENADOR da disciplina: ";
        if (!(std::cin >> coordenadorId)) {

            std::cerr << "\nERRO: Entrada inva'lida. Tente novamente.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; //se a entrada for invalida, volta para o inicio do loop
        }

        //procura o professor com o id escolhido
        for (auto p : professoresDisponiveis) {
            if (p->getId() == coordenadorId) {
                profCoordenador = p;
                break;
            }
        }
        //se nao encontrar, mostra o erro
        if (profCoordenador == nullptr) {
            std::cerr << "\nERRO: ID de professor inva'lido. Tente novamente.\n";
        }
    }

    //se o professor nao for coordenador de disciplina, promove ele
    if (profCoordenador->getTipo() == "PROFESSOR") {
        profCoordenador->setTipo("COORDENADOR_DISCIPLINA");
        std::cout << "\nProfessor " << profCoordenador->getNome() << " promovido para COORDENADOR_DISCIPLINA.\n";
    }

    int novaDiscId = ProximoIdDisciplinas(_disciplinas); //gera novo id para a disciplina
    int profIdResponsavel = coordenadorId; //o professor responsavel e' o coordenador
    
    //cria a disciplina e adiciona ao vetor
    _disciplinas.emplace_back(novaDiscId, codigo, nome, profIdResponsavel, coordenadorId);
    std::cout << "Disciplina cadastrada (ID=" << novaDiscId << ")\n";
    
    //cria automaticamente a Turma 1 para a disciplina cadastrada
    int novaTurmaId = ProximoIdTurmas(_turmas);
    std::string codigoTurma = "1";
    int professorTurmaId = coordenadorId; //o coordenador e' o professor da Turma 1

    //adiciona a nova turma ao vetor
    _turmas.emplace_back(novaTurmaId, novaDiscId, codigoTurma, professorTurmaId);
    std::cout << "Turma 1 automatica cadastrada (ID=" << novaTurmaId << "). Professor: " << profCoordenador->getNome() << ".\n";

    arquivo.salvarDisciplinas(_disciplinas); //salva as disciplinas no arquivo
}
//cadastra uma nova turma
void SistemaAvaliacao::cadastrarTurma() {
    
    //variveis para armazenar os dados da turma
    int disciplinaId = -1; //inicializa com valor invalido
    int professorId = -1;
    std::string codigoTurma;
    std::string disciplinaIdStr; //para ler o id da disciplina como string

    try {
        if (_disciplinas.empty()) {
            throw "Nao e' possivel cadastrar turmas. Nao existem disciplinas cadastradas. ";
        }

        std::cout << "\nDisciplinas disponiveis:\n";
        for (auto &d : _disciplinas) {
            std::cout << d.getId() << " - " << d.getCodigo() << " " << d.getNome() << '\n';
        }
        
        bool idDisciplinaValido = false; //flag para verificar se o id da disciplina e valido

        while (!idDisciplinaValido) {
            std::cout << "Escolha ID da disciplina: ";
            
            // leitura como string para evitar falhas de estado do std::cin

            if (!(std::cin >> disciplinaIdStr)) {
                //tratamento de erro de leitura de cin
                std::cerr << "ERRO: Falha de leitura. Tente novamente.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            //consome o resto da linha
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            try {
                //tenta converter a string para inteiro
                disciplinaId = std::stoi(disciplinaIdStr);
                
                //verifica se o id da disciplina realmente existe
                bool disciplinaExiste = false;
                for (const auto& d : _disciplinas) {
                    if (d.getId() == disciplinaId) {
                        disciplinaExiste = true;
                        break;
                    }
                }
                //se existir, marca como valido
                if (disciplinaExiste) {
                    idDisciplinaValido = true; //sai do loop
                } else {
                    std::cerr << "ERRO: Disciplina com ID " << disciplinaId << " nao encontrada. Tente novamente.\n";
                }
                //se nao existir, mostra o erro
            } catch (const std::invalid_argument& e) {
                std::cerr << "ERRO: O ID da disciplina deve ser um numero inteiro valido. Tente novamente.\n";
            } catch (const std::out_of_range& e) {
                std::cerr << "ERRO: Numero fora do intervalo permitido. Tente novamente.\n";
            }
        }

        //leitura e validacao do codigo da turma
        bool codigoJaExiste;//flag para verificar se o codigo ja existe

        do {
            std::cout << "Codigo da turma (ex: 1): ";
            std::cin >> codigoTurma;
            
            //consome o resto da linha para limpar o buffer apos a leitura da string
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            codigoJaExiste = false; //inicializa como falso

            for (const auto& t : _turmas) {
                // Verifica se o codigo ja existe PARA A MESMA DISCIPLINA
                if (t.getDisciplinaId() == disciplinaId && t.getCodigoTurma() == codigoTurma) {
                    codigoJaExiste = true; //se encontrar, marca como existente
                    std::cerr << "ERRO: Ja existe uma turma com esse codigo (" << codigoTurma << ") para esta disciplina. Tente outro.\n";
                    break;
                }
            }

        } while (codigoJaExiste);
        
        //leitura e validacao do id do professor
        bool idProfessorValido = false;
        std::string professorIdStr; //para ler o id do professor como string
        
        while (!idProfessorValido) {

            std::cout << "\nProfessores disponiveis:\n";
            bool professorEncontrado = false; //flag para verificar se ha professores disponiveis

            for (auto u : _usuarios) {
                //mostra apenas professores e coordenadores de disciplina
                if (u->getTipo() == "PROFESSOR" || u->getTipo() == "COORDENADOR_DISCIPLINA") {
                    std::cout << u->getId() << " - " << u->getNome() << "\n";
                    professorEncontrado = true;
                }
            }
            //se nao houver professores ou coordenadores disponiveis, lanca excecao
            if (!professorEncontrado) {
                 throw "Nao ha professores ou coordenadores disponiveis para atribuicao.";
            }

            std::cout << "Escolha ID do professor para a turma: ";
            
            //leitura como string para evitar erro
            if (!(std::cin >> professorIdStr)) {
                std::cerr << "ERRO: Falha de leitura. Tente novamente.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            try { //tenta converter a string para inteiro
                professorId = std::stoi(professorIdStr);
                
                //verifica se o professor ID realmente existe e é do tipo correto
                bool professorExiste = false;
                for (const auto* u : _usuarios) {
                    if (u->getId() == professorId && 
                        (u->getTipo() == "PROFESSOR" || u->getTipo() == "COORDENADOR_DISCIPLINA")) {
                        professorExiste = true;
                        break;
                    }
                }
                //se existir, marca como valido
                if (professorExiste) {
                    idProfessorValido = true;
                } else {
                    std::cerr << "ERRO: Usuario com ID " << professorId << " nao e' um professor ou nao existe. Tente novamente.\n";
                }
            //se nao existir, mostra o erro
            } catch (const std::invalid_argument& e) {
                std::cerr << "ERRO: O ID do professor deve ser um numero inteiro valido. Tente novamente.\n";
            } catch (const std::out_of_range& e) {
                std::cerr << "ERRO: Numero fora do intervalo permitido. Tente novamente.\n";
            }
        }
        
        //cria a nova turma e adiciona ao vetor
        int id = ProximoIdTurmas(_turmas);
        _turmas.emplace_back(id, disciplinaId, codigoTurma, professorId);
        std::cout << "\nTurma cadastrada com sucesso (ID=" << id << ")\n";

        arquivo.salvarTurmas(_turmas);
        
    }
    //tratamentos de excecoes
    catch (std::invalid_argument& e) {
        //erro de conversao de string para inteiro
        std::cerr << "ERRO: Erro de conversao de entrada. Tente novamente\n";
    }
    catch (const std::runtime_error& e) {
        //para outros erros de tempo de execucao
        std::cerr << "ERRO de execucao: " << e.what() << '\n';
    }
    catch (const char* msg) {
        //captura a excecao de string para erros
        std::cerr << "ERRO: " << msg << '\n';
    }
}

void SistemaAvaliacao::matricularAluno() {

    //verifica se ha alunos e turmas cadastradas
    std::vector<Usuario*> alunos;
    for(auto u : _usuarios) {
        if (u->getTipo() == "ALUNO") {
            alunos.push_back(u);
        }
    }
    
    if (alunos.empty()) {
        std::cerr << "ERRO: Nenhum aluno cadastrado no sistema.\n";
        return;
    }

    if (_turmas.empty()) {
        std::cerr << "ERRO: Nenhuma turma cadastrada no sistema.\n";
        return;
    }


    //entao inicia o processo de matricula
    std::cout << "\nAlunos disponi'veis:\n";
    for(const auto* a : alunos) {
        std::cout << "ID: " << a->getId() << " - " << a->getNome() << " (" << a->getemail() << ")\n";
    }

    int alunoId;
    Usuario* alunoSelecionado = nullptr; //ponteiro para o aluno selecionado
    while(alunoSelecionado == nullptr) { //enquanto nao encontrar um aluno valido

        std::cout << "Escolha o ID do aluno para matricular: ";
        if (!(std::cin >> alunoId)) {
            std::cerr << "ERRO: Entrada inva'lida. Tente novamente.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        //procura o aluno com o id escolhido
        for(auto a : alunos) {
            if (a->getId() == alunoId) {
                alunoSelecionado = a;
                break;
            }
        }
        if (alunoSelecionado == nullptr) {
            std::cerr << "ERRO: ID de aluno inva'lido!! Tente novamente.\n";
        }
    }

    //entao escolhe a disciplina
    int disciplinaIdEscolhida = -1;

    // lista disciplinas
    std::cout << "\nDisciplinas disponi'veis:\n";
    for (const auto &d : _disciplinas) {
        std::cout << d.getId() << " - " << d.getCodigo() << " - " << d.getNome() << '\n';
    }

    bool discValida = false; //flag para verificar se a disciplina e valida

    while (!discValida) { //enquanto nao encontrar uma disciplina valida
        std::cout << "Escolha o ID da disciplina para matricular o aluno: ";
        if (!(std::cin >> disciplinaIdEscolhida)) {
            std::cerr << "ERRO: Entrada inva'lida. Tente novamente.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
            //procura a disciplina com o id escolhido
        for (const auto &d : _disciplinas) {
            if (d.getId() == disciplinaIdEscolhida) {
                discValida = true;
                break;
            }
        }
        if (!discValida) {
            std::cerr << "ERRO: ID de disciplina inva'lido. Tente novamente.\n";
        }
    }
    // lista turmas da disciplina escolhida
    std::vector<Turma*> turmasDaDisciplina;
    std::cout << "\nTurmas disponi'veis para essa disciplina:\n";
    for (auto &t : _turmas) {
        if (t.getDisciplinaId() == disciplinaIdEscolhida) {
            std::cout << "ID: " << t.getId()
                    << " - Turma " << t.getCodigoTurma() << std::endl;
            turmasDaDisciplina.push_back(&t);
        }
    }
    //verifica se ha turmas para a disciplina escolhida
    if (turmasDaDisciplina.empty()) {
        std::cerr << "ERRO: Nao ha turmas cadastradas para essa disciplina.\n";
        return;
    }
        //entao escolhe a turma
        int turmaId;
        Turma* turmaSelecionada = nullptr;
        while (turmaSelecionada == nullptr) {
            std::cout << "Escolha o ID da turma para matricular: ";
            if (!(std::cin >> turmaId)) {
                std::cerr << "ERRO: Entrada inva'lida. Tente novamente.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
        //procura a turma com o id escolhido
            for (auto t : turmasDaDisciplina) {
                if (t->getId() == turmaId) {
                    turmaSelecionada = t;
                    break;
                }
            }
            if (turmaSelecionada == nullptr) {
                std::cerr << "ERRO: ID de turma inva'lido para essa disciplina. Tente novamente.\n";
            }
        }
        
        //verifica se o aluno ja esta matriculado na turma selecionada
        bool jaMatriculado = false;
        for (const auto* t : alunoSelecionado->getMinhasDisciplinas()) {
            if (t->getId() == turmaSelecionada->getId()) {
                jaMatriculado = true;
                break;
            }
        }
        //se ja estiver matriculado, mostra o erro
        if (jaMatriculado) {
            std::cout << "ERRO: Estudante " << alunoSelecionado->getNome() << " ja' esta' matriculado nesta turma.\n";
            return;
        }

        int disciplinaDaTurmaSelecionada = turmaSelecionada->getDisciplinaId(); //pega o id da disciplina da turma selecionada

        //verifica se o aluno ja esta matriculado em outra turma da mesma disciplina
        bool jaMatriculadoNaDisciplina = false;

        for (const auto* t : alunoSelecionado->getMinhasDisciplinas()) {
            if (t->getDisciplinaId() == disciplinaDaTurmaSelecionada) {
                jaMatriculadoNaDisciplina = true;
                break;
            }
        }

        if (jaMatriculadoNaDisciplina) {
            std::cout << "\nERRO: Estudante "
                    << alunoSelecionado->getNome()
                    << " ja' esta' matriculado em outra turma dessa mesma disciplina.\n";
            return;
        }


        //matricula o aluno na turma selecionada
        alunoSelecionado->addTurma(turmaSelecionada);

        std::cout << "\nAluno " << alunoSelecionado->getNome() << " matriculado com sucesso na Turma " << turmaSelecionada->getCodigoTurma() << ".\n";

        arquivo.salvarMatriculas(_usuarios);
}
//-----------------------------------------------------------------------

// AVALIACOES-------------------------------------------------------------
void SistemaAvaliacao::avaliarDisciplina(Usuario* u) {
    //vetor de turmas em que o usuario esta matriculado
    std::vector<Turma*> turmasemaildas = u->getMinhasDisciplinas();

    if (turmasemaildas.empty()) { //se nao estiver matriculado em nenhuma turma
        throw "Voce nao esta matriculado em nenhuma disciplina. Nenhuma disciplina disponivel para avaliacao.\n";
    }

    std::vector<int> disciplinasIds;
    for (const auto* t : turmasemaildas) {
        disciplinasIds.push_back(t->getDisciplinaId()); // Turma sabe sua Disciplina
    }

    // lista disciplinas disponiveis para avaliacao
    std::cout << "Disciplinas matriculadas disponiveis para avaliacao:\n";
    std::vector<Disciplina*> disciplinasDisponiveis;
    for (int discId : disciplinasIds) {
        for (auto &d : _disciplinas) {
            if (d.getId() == discId) {
                std::cout << d.getId() << " - " << d.getCodigo() << " " << d.getNome() << '\n';
                disciplinasDisponiveis.push_back(&d);
                break;
            }
        }
    }

    int idDisc = -1; // inicializa com valor invalido
    std::string idDiscStr; // para ler o id da disciplina como string
    bool idValido = false; // flag para verificar se o id da disciplina e valido

    do {
        idDisc = -1;// reseta o id
        idValido = false; // reseta a flag
        std::cout << "Escolha ID da disciplina: ";
        std::cin >> idDiscStr;

        try { // tenta converter a string para inteiro
            idDisc = std::stoi(idDiscStr);
        }
        catch (std::invalid_argument& e) {std::cerr << "ID invalido. Tente novamente.\n"; 
            continue;
        }
        catch (std::out_of_range& e) {std::cerr << "ID invalido. Tente novamente.\n"; 
            continue;
        } 
        
        // verifica se o id escolhido esta na lista de disciplinas disponiveis
        for(const auto* d : disciplinasDisponiveis) {
            if (d->getId() == idDisc) {
                idValido = true;
                break;
            }
        }

        if (!idValido) {
            std::cerr << "ID invalido. Tente novamente.\n";
        }
    } while (!idValido);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // perguntas da avaliacao
    int nota[5];
    std::cout << "Como voce avalia a disciplina? (0-5)\n";
    std::cout << "Sendo 0 discordo totalmente, e 5 concordo plenamente.\n";

    nota[0] = lerInteiroComExcecao("Em relacao a ementa: ");
    nota[1] = lerInteiroComExcecao("\nVoce considera o conteudo da disciplina relevante para sua formacao?: ");
    nota[2] = lerInteiroComExcecao("\nOs meios de avaliar a aprendizagem foram eficazes? (ex.: provas e seminarios): ");
    nota[3] = lerInteiroComExcecao("\nO conteudo planejado foi compativel com a carga horaria da disciplina?: ");
    nota[4] = lerInteiroComExcecao("\nA disciplina complementa/agrega conhecimento para o restante do curso?: ");

    std::string comentario;
    std::cout << "\nDeixe aqui elogios, sugestoes ou cri'ticas para aprimoramento da disciplina: ";
    std::getline(std::cin >> std::ws, comentario);

    // calcula a media das notas
    double mediaNotas = 0.0;
    for (int i=0; i<5; i++)
        mediaNotas += nota[i];
        mediaNotas /= 5.0;

    // cria a avaliacao e adiciona ao vetor
    int id = ProximoIdAvaliacoes(_avaliacoes);
    _avaliacoes.emplace_back(id, idDisc, "DISCIPLINA", mediaNotas, comentario, getDataAtual());
    std::cout << "Avaliacao de disciplina registrada.\n";

    arquivo.salvarAvaliacoes(_avaliacoes);
}

void SistemaAvaliacao::avaliarProfessor(Usuario* u) {
    std::vector<Turma*> turmasMatriculadas = u->getMinhasDisciplinas();

    if (turmasMatriculadas.empty()) {
        throw "Voce nao esta' matriculado em nenhuma turma. Nenhum professor disponi'vel para avaliacao.\n";
    }

    //mapeia professores para as disciplinas que lecionam nas turmas em que o aluno esta matriculado
    std::map<int, std::vector<std::pair<int, std::string>>> professoresDisciplinas; 

    for (const auto* t : turmasMatriculadas) {
        int profId = t->getProfessorId();
        int discId = t->getDisciplinaId();
        Disciplina* disc = getDisciplinaPorID(discId);

        if (profId > 0 && disc) { 
            bool jaAdicionado = false;
            // verifica se a disciplina já foi adicionada para este professor
            for (const auto& pair : professoresDisciplinas[profId]) {
                if (pair.first == discId) {
                    jaAdicionado = true;
                    break;
                }
            }
            if (!jaAdicionado) {
                professoresDisciplinas[profId].push_back({discId, disc->getCodigo()});
            }
        }
    }

    //depois daqui, mesmo processo de escolha de professor e disciplina

    std::cout << "Professores disponiveis para avaliacao:\n";
    std::map<int, Usuario*> listaProfessores;

    for (auto const& [profId, discs] : professoresDisciplinas) {
        Usuario* usuario = getUsuarioPorID(profId);
        if (usuario && (usuario->getTipo() == "PROFESSOR" || usuario->getTipo() == "COORDENADOR_DISCIPLINA" || usuario->getTipo() == "COORDENADOR_DO_CURSO")) {
            std::cout << usuario->getId() << " - " << usuario->getNome() << '\n';
            listaProfessores[profId] = usuario;
        }
    }
    
    bool idProfValido = false;
    int profId = -1;
    std::string profIdStr;

    do {
        profId = -1;
        idProfValido = false;

        std::cout << "Escolha ID do professor: "; 
        std::cin >> profIdStr;

        try {profId = std::stoi(profIdStr);}
        catch (std::invalid_argument& e) {std::cerr << "ID de professor invalido. Tente novamente.\n"; continue;}
        catch (std::out_of_range& e) {std::cerr << "ID de professor invalido. Tente novamente.\n"; continue;}

        if (listaProfessores.count(profId)) {
            idProfValido = true;
        } else {
            std::cerr << "ID de professor invalido. Tente novamente\n";
        }
    } while (!idProfValido);

    //mostra as disciplinas do professor escolhido
    std::cout << "\nDisciplinas em que " << listaProfessores[profId]->getNome() << " leciona (Escolha para qual avaliar):\n";
    
    const auto& disciplinasDoProfessor = professoresDisciplinas[profId];
    for (const auto& pair : disciplinasDoProfessor) {
        std::cout << pair.first << " - " << pair.second << '\n';
    }
    
    bool idDiscValido = false;
    int discId = -1;
    std::string discIdStr;
    std::string codigoDisciplina;
    do {
        discId = -1;
        idDiscValido = false;
        std::cout << "Escolha ID da disciplina para avaliar: ";
        std::cin >> discIdStr;

        try {discId = std::stoi(discIdStr);}
        catch (std::invalid_argument& e) {std::cerr << "ID de disciplina invalido para este professor. Tente novamente.\n"; continue;}
        catch (std::out_of_range& e) {std::cerr << "ID de disciplina invalido para este professor. Tente novamente.\n"; continue;}

        for (const auto& pair : disciplinasDoProfessor) {
            if (pair.first == discId) {
                idDiscValido = true;
                codigoDisciplina = pair.second;
                break;
            }
        }
        if (!idDiscValido) {
            std::cerr << "ID de disciplina invalido para este professor. Tente novamente.\n";
        }
    } while (!idDiscValido);
    
    // Agora temos o profId e discId escolhidos
    //mostra a avaliacao que sera feita
    std::cout << "\nAvaliacao para o Professor " << listaProfessores[profId]->getNome() << " na disciplina " << codigoDisciplina << ".\n";


    //continua com a coleta de notas, mesma coisa
    int nota[5]; 
    std::cout << "Como voce avalia o professor? (0-5)\n";
    std::cout << "Sendo 0 discordo totalmente, e 5 concordo plenamente.\n";
    
    nota[0] = lerInteiroComExcecao("O professor forneceu feedback das avaliacoes?: ");
    nota[1] = lerInteiroComExcecao("\nA metodologia de ensino do professor favoreceu a aprendizagem?: ");
    nota[2] = lerInteiroComExcecao("\nO professor incentivou os alunos a participarem das aulas?: ");
    nota[3] = lerInteiroComExcecao("\nO professor demonstrou domi'nio no conteu'do da disciplina?: ");
    nota[4] = lerInteiroComExcecao("\nA dida'tica do professor foi objetiva e clara?: ");

    std::string comentario; 
    std::cout << "\nDeixe aqui elogios, sugestoes ou cri'ticas para aprimoramento da metodologia utilizada: ";
    std::getline(std::cin >> std::ws, comentario);

    double mediaNotas = 0.0;
    for (int i=0; i<5; i++)
        mediaNotas += nota[i];
        mediaNotas /= 5.0;
    int id = ProximoIdAvaliacoes(_avaliacoes);

    // cria a avaliacao e adiciona ao vetor
    _avaliacoes.emplace_back(id, profId, "PROFESSOR", mediaNotas, comentario, getDataAtual());
    std::cout << "Avaliacao de professor registrada.\n";

    arquivo.salvarAvaliacoes(_avaliacoes);
}

void SistemaAvaliacao::avaliarTurma(Usuario* u) {

    //mesmo processo de filtrar turmas ministradas pelo professor igual as outras avaliacoes
    if (_turmas.empty()) { 
        throw "Nenhuma turma cadastrada no sistema.\n";
    }

    int profId = u->getId();
    std::vector<Turma*> turmasMinistradas;
    for (auto &t : _turmas) {
        if (t.getProfessorId() == profId) {
            turmasMinistradas.push_back(&t);
        }
    }

    if (turmasMinistradas.empty()) {
        throw "Nao e' possi'vel fazer uma avaliacao, pois voce nao leciona em nenhuma turma.\n";
    }

    std::cout << "Suas Turmas (Turmas que voce ministra):\n";
    for (auto &t : turmasMinistradas) {
        std::string discInfo = "Disciplina Desconhecida";
        //busca o codigo da disciplina
        for (auto &d : _disciplinas) {
            if (d.getId() == t->getDisciplinaId()) {
                discInfo = d.getCodigo();
                break;
            }
        }
        std::cout << t->getId() << " - Disciplina: " << discInfo << " | Turma: " << t->getCodigoTurma() << '\n';
    }


    int turmaId = -1; // inicializa com valor invalido
    bool idValido = false;
    std::string turmaIdStr;

    do {
        turmaId = -1;
        idValido = false;

        std::cout << "Escolha ID da turma: "; 
        std::cin >> turmaIdStr;

        try {
            turmaId = std::stoi(turmaIdStr);
        }
        catch (std::invalid_argument& e) {
            std::cerr << "\nID de turma invalido ou voce nao ministra esta turma.\n"; 
            continue;
        }
        catch (std::out_of_range& e) {
            std::cerr << "\nID de turma invalido ou voce nao ministra esta turma.\n"; 
            continue;
        }

        for(const auto* t : turmasMinistradas) {
            if (t->getId() == turmaId) {
                idValido = true;
                break;
            }
        }

        if (!idValido) {
            std::cerr << "\nID de turma invalido ou voce nao ministra esta turma.\n";
            continue;
        }

    } while (!idValido);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        int nota[5]; 
        std::cout << "\nComo voce avalia a turma? (0-5)\n";
        std::cout << "Sendo 0 discordo totalmente e 5 concordo plenamente.\n";

        nota[0] = lerInteiroComExcecao("\nA turma e' participativa?: ");
        nota[1] = lerInteiroComExcecao("\nVoce considera que os alunos se preparavam com antecendencia?: ");
        nota[2] = lerInteiroComExcecao("\nVoce considera que a turma possui lacuna quanto aos pre'-requisitos da disciplina?: ");
        nota[3] = lerInteiroComExcecao("\nOs alunos foram assi'duos?: ");
        nota[4] = lerInteiroComExcecao("\nA turma se mostrou interessada em integrar os conhecimentos a sua formacao profissional?: ");

        std::string comentario; 
        std::cout << "\nComentario: "; 
        std::getline(std::cin >> std::ws, comentario);

        double mediaNotas = 0;
        for (int i=0; i<5; i++)
            mediaNotas += nota[i];
            mediaNotas /= 5.0;
        int id = ProximoIdAvaliacoes(_avaliacoes);
        // cria a avaliacao e adiciona ao vetor
        _avaliacoes.emplace_back(id, turmaId, "TURMA", mediaNotas, comentario, getDataAtual());
        std::cout << "Avaliacao de turma registrada.\n";

        arquivo.salvarAvaliacoes(_avaliacoes);
}
//-----------------------------------------------------------------------

//LISTAGEM
void SistemaAvaliacao::visualizarAvaliacoesProfessor(Usuario* u) {
    //verifica o tipo de usuario e seu id
    std::string tipoUsuario = u->getTipo();
    int userId = u->getId();

    //para coordenadores de disciplina, coleta os ids das disciplinas que coordenam
    std::vector<int> discIdsCoordenadas;
    if (tipoUsuario == "COORDENADOR_DISCIPLINA") {
        for (const auto& d : _disciplinas) {
            if (d.getCoordenadorDiscId() == userId) {
                discIdsCoordenadas.push_back(d.getId());
            }
        }
    }

    //inicia a exibicao das avaliacoes
    std::cout << "\n===== VISUALIZACAO DE AVALIACOES (" << (tipoUsuario == "COORDENADOR_DISCIPLINA" ? "COORDENADOR" : "PROFESSOR") << ") =====\n";
    std::cout << std::fixed << std::setprecision(2);

    //contador de avaliacoes exibidas
    int count = 0;

    for (const auto& a : _avaliacoes) {
        bool show = false;
        std::string alvoInfo = "ID: " + std::to_string(a.getAlvoId()); //info padrao do alvo

        if (tipoUsuario == "COORDENADOR_DISCIPLINA") {
            
            if (a.getTipo() == "DISCIPLINA") {
                if (std::find(discIdsCoordenadas.begin(), discIdsCoordenadas.end(), a.getAlvoId()) != discIdsCoordenadas.end()) {
                    show = true; // Avaliacoes destinadas às disciplinas que coordena
                }

            } else if (a.getTipo() == "PROFESSOR") {
                //verifica se o professor avaliado leciona alguma disciplina que o coordenador coordena
                for (const auto& dId : discIdsCoordenadas) {
                    Disciplina* d = getDisciplinaPorID(dId);
                    if (d && d->getProfessorId() == a.getAlvoId()) {
                        show = true;
                        break;
                    }
                }
            } else if (a.getTipo() == "TURMA") {
                //verifica se a turma avaliada pertence a alguma disciplina que o coordenador coordena
                Turma* t = getTurmaPorID(a.getAlvoId());
                if (t && std::find(discIdsCoordenadas.begin(), discIdsCoordenadas.end(), t->getDisciplinaId()) != discIdsCoordenadas.end()) {
                    show = true;
                }
            }

        } else if (tipoUsuario == "PROFESSOR") {
            
            if (a.getTipo() == "PROFESSOR" && a.getAlvoId() == userId) {
                show = true; //avaliacoes destinadas a ele
            } else if (a.getTipo() == "DISCIPLINA") {
                // Verifica se ele e' o professor principal de alguma turma dessa disciplina
                bool leciona = false;
                for (const auto& t : _turmas) {
                    if (t.getDisciplinaId() == a.getAlvoId() && t.getProfessorId() == userId) {
                        leciona = true;
                        break;
                    }
                }
                if (leciona) {
                    show = true; //avaliacoes destinadas a disciplina que ele leciona
                }
            }
        }
        
        // Se a avaliacao deve ser exibida, formata a informacao do alvo
        if (show) {
            count++; //incrementa o contador de avaliacoes exibidas

            //formata a informacao do alvo
            if (a.getTipo() == "TURMA") {
                Turma* t = getTurmaPorID(a.getAlvoId());
                Disciplina* d = t ? getDisciplinaPorID(t->getDisciplinaId()) : nullptr;
                alvoInfo = (d ? d->getCodigo() : "DESC.") + " - Turma " + (t ? t->getCodigoTurma() : "DESC.");
            } else if (a.getTipo() == "DISCIPLINA") {
                Disciplina* d = getDisciplinaPorID(a.getAlvoId());
                alvoInfo = "Disciplina: " + (d ? d->getCodigo() : "DESC.");
            } else if (a.getTipo() == "PROFESSOR") {
                Usuario* p = getUsuarioPorID(a.getAlvoId());
                alvoInfo = "Professor: " + (p ? p->getNome() : "DESC.");
            }
            //exibe a avaliacao
            std::cout << "ID: " << a.getId() << " | Tipo: " << a.getTipo() << " | Alvo: " << alvoInfo << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
    }
    if (count == 0) {
        std::cout << "Nenhuma avaliacao encontrada com base nas suas credenciais.\n";
    }
}

void SistemaAvaliacao::visualizarMediasAluno(Usuario* u) {
    // mesmo processo de visualizacao de avaliacoes do professor, mas agora para as medias dos alunos
    if (u->getTipo() != "ALUNO") {
        std::cerr << "ERRO: Funcionalidade restrita a alunos.\n";
        return;
    }

    std::vector<Turma*> turmasAluno = u->getMinhasDisciplinas();
    if (turmasAluno.empty()) {
        std::cout << "Voce não está matriculado em nenhuma turma.\n";
        return;
    }

    // Usar um mapa para garantir que cada professor/disciplina seja listado apenas uma vez
    std::map<int, std::pair<std::string, std::vector<double>>> mediasPorProfessor; 
    std::map<int, std::pair<std::string, std::vector<double>>> mediasPorDisciplina; 

    for (const auto* turma : turmasAluno) {
        int profId = turma->getProfessorId();
        int discId = turma->getDisciplinaId();
        Disciplina* disc = getDisciplinaPorID(discId);
        Usuario* prof = getUsuarioPorID(profId);

        // Processar Avaliacoes de PROFESSOR
        if (prof) {
            if (mediasPorProfessor.find(profId) == mediasPorProfessor.end()) {
                mediasPorProfessor[profId].first = prof->getNome();
            }
        }

        // Processar Avaliacoes de DISCIPLINA
        if (disc) {
            if (mediasPorDisciplina.find(discId) == mediasPorDisciplina.end()) {
                mediasPorDisciplina[discId].first = disc->getCodigo();
            }
        }
    }

    // Coletar todas as notas
    for (const auto& a : _avaliacoes) {
        if (a.getTipo() == "PROFESSOR" && mediasPorProfessor.count(a.getAlvoId())) {
            mediasPorProfessor[a.getAlvoId()].second.push_back(a.getNota());
        } else if (a.getTipo() == "DISCIPLINA" && mediasPorDisciplina.count(a.getAlvoId())) {
            mediasPorDisciplina[a.getAlvoId()].second.push_back(a.getNota());
        }
    }

    std::cout << "\n===== MEDIAS DE AVALIAÇÃO (ALUNO) =====\n";
    std::cout << std::fixed << std::setprecision(2);

    // Exibir medias de Professor
    std::cout << "\n--- Media de Avaliacoes de Professores ---\n";
    for (const auto& pair : mediasPorProfessor) {
        if (!pair.second.second.empty()) {
            double media = std::accumulate(pair.second.second.begin(), pair.second.second.end(), 0.0) / pair.second.second.size();
            std::cout << "Professor: " << pair.second.first << " | Media: " << media << '\n';
        } else {
            std::cout << "Professor: " << pair.second.first << " | Media: N/A (Sem avaliacoes)\n";
        }
    }
    
    // Exibir medias de Disciplina
    std::cout << "\n--- Media de Avaliacoes de Disciplinas ---\n";
    for (const auto& pair : mediasPorDisciplina) {
        if (!pair.second.second.empty()) {
            double media = std::accumulate(pair.second.second.begin(), pair.second.second.end(), 0.0) / pair.second.second.size();
            std::cout << "Disciplina: " << pair.second.first << " | Media: " << media << '\n';
        } else {
            std::cout << "Disciplina: " << pair.second.first << " | Media: N/A (Sem avaliacoes)\n";
        }
    }
}

//RELATORIOS
//relatorio geral para o Coordenador de Curso
void SistemaAvaliacao::relatorioGeralCoordenador() {
    //imprime os relatorios gerais

    std::cout << "\n===== RELATORIO GERAL (COORDENADOR DE CURSO) =====\n";
    std::cout << std::fixed << std::setprecision(2);

    // Estrutura para armazenar todas as notas e comentários agrupados
    std::map<std::string, std::map<int, std::pair<std::string, std::vector<std::pair<double, std::string>>>>> relatorio; 

    //mapeamento de IDs para npmes/coigos para facilitar a exibicao

    auto MapeamentodeIds = [&](const std::string& tipo, int id) -> std::string {
        if (tipo == "DISCIPLINA") {
            Disciplina* d = getDisciplinaPorID(id);
            return d ? d->getCodigo() + " (" + d->getNome() + ")" : "Disciplina Desconhecida";
        } else if (tipo == "PROFESSOR") {
            Usuario* u = getUsuarioPorID(id);
            return u ? u->getNome() : "Professor Desconhecido";
        } else if (tipo == "TURMA") {
            Turma* t = getTurmaPorID(id);
            if (t) {
                Disciplina* d = getDisciplinaPorID(t->getDisciplinaId());
                return (d ? d->getCodigo() : "DESC.") + " - Turma " + t->getCodigoTurma();
            }
            return "Turma Desconhecida";
        }
        return "Alvo ID: " + std::to_string(id); // Caso padrao
    };

    for (const auto& a : _avaliacoes) {
        std::string alvoNome = MapeamentodeIds(a.getTipo(), a.getAlvoId());
        
        relatorio[a.getTipo()][a.getAlvoId()].first = alvoNome;
        relatorio[a.getTipo()][a.getAlvoId()].second.push_back({a.getNota(), a.getComentario()});
    }

    // Exibir o relatorio
    for (const auto& tipoPair : relatorio) {
        std::cout << "\n--- Avaliacoes do Tipo: " << tipoPair.first << " ---\n";
        // Para cada alvo dentro do tipo
        for (const auto& alvoPair : tipoPair.second) {
            const std::string& alvoNome = alvoPair.second.first;
            const auto& notasComentarios = alvoPair.second.second;
            
            if (!notasComentarios.empty()) {
                double totalNotas = 0.0;
                for (const auto& nc : notasComentarios) totalNotas += nc.first;
                double media = totalNotas / notasComentarios.size();

                std::cout << "\nAlvo: " << alvoNome << " (Total Avaliacoes: " << notasComentarios.size() << ")\n";
                std::cout << "Media: " << media << '\n';
                std::cout << "Comentarios:\n";
                
                for (const auto& nc : notasComentarios) {
                    std::cout << "  - [Nota " << nc.first << "]: \"" << nc.second << "\"\n";
                }
            }
        }
    }

    if (_avaliacoes.empty()) {
        std::cout << "Nenhuma avaliacao registrada no sistema.\n";
    }
}
//listar avaliacoes por tipo
void SistemaAvaliacao::listarAvaliacoes(const std::string &tipo) {

    if (tipo == "GERAL") {
        if (_avaliacoes.empty()) {
            std::cerr << "Nenhuma avaliacao registrada no sistema.\n";
            return;    
        }
        std::cout << " Avaliacoes disponiveis : \n";

        for (auto &a : _avaliacoes) {

            // logica para identificar e formatar as informações do alvo
            //processo parecido com os anteriores
            std::string alvoInfo;
            if (a.getTipo() == "TURMA") {
                Turma* turma = nullptr;
                for (auto &t : _turmas) {
                    if (t.getId() == a.getAlvoId()) {
                        turma = &t;
                        break;
                    }
                }
                if (turma) {
                    Disciplina* disciplina = nullptr;
                    for (auto &d : _disciplinas) {
                        if (d.getId() == turma->getDisciplinaId()) {
                            disciplina = &d;
                            break;
                        }
                    }
                    if (disciplina) {
                        alvoInfo = disciplina->getCodigo() + " - Turma " + turma->getCodigoTurma();
                    } else {
                        alvoInfo = "Turma ID: " + std::to_string(a.getAlvoId()) + " (Disciplina Desconhecida)";
                    }
                } else {
                     alvoInfo = "Turma ID Desconhecido: " + std::to_string(a.getAlvoId());
                }

            } else if (a.getTipo() == "DISCIPLINA") {
                Disciplina* disciplina = nullptr;
                for (auto &d : _disciplinas) {
                    if (d.getId() == a.getAlvoId()) {
                        disciplina = &d;
                        break;
                    }
                }
                if (disciplina) {
                    alvoInfo = "Disciplina: " + disciplina->getCodigo() + " (" + disciplina->getNome() + ")";
                } else {
                    alvoInfo = "Disciplina ID Desconhecida: " + std::to_string(a.getAlvoId());
                }
            } else if (a.getTipo() == "PROFESSOR") {
                Usuario* professor = nullptr;
                for (auto *u : _usuarios) {
                    if (u->getId() == a.getAlvoId() && (u->getTipo() == "PROFESSOR" || u->getTipo() == "COORDENADOR_DISCIPLINA" || u->getTipo() == "COORDENADOR_DO_CURSO")) {
                        professor = u;
                        break;
                    }
                }
                if (professor) {
                    alvoInfo = "Professor: " + professor->getNome() + " (ID: " + std::to_string(a.getAlvoId()) + ")";
                } else {
                    alvoInfo = "Professor ID Desconhecido: " + std::to_string(a.getAlvoId());
                }
            } else {
                 alvoInfo = "Alvo ID: " + std::to_string(a.getAlvoId());
            }

            std::cout << "ID: " << a.getId() << " | Tipo: " << a.getTipo() << " | Alvo: " << alvoInfo << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
        return;
    }

    // filtrar por tipo
    std::cout << " Avaliacoes do tipo " << tipo << std::endl;
    for (auto &a : _avaliacoes) {
        if (a.getTipo() == tipo) {
            std::string alvoInfo;
            
            if (a.getTipo() == "TURMA") {
                Turma* turma = nullptr;
                for (auto &t : _turmas) {
                    if (t.getId() == a.getAlvoId()) {
                        turma = &t;
                        break;
                    }
                }
                if (turma) {
                    Disciplina* disciplina = nullptr;
                    for (auto &d : _disciplinas) {
                        if (d.getId() == turma->getDisciplinaId()) {
                            disciplina = &d;
                            break;
                        }
                    }
                    if (disciplina) {
                        alvoInfo = disciplina->getCodigo() + " - Turma " + turma->getCodigoTurma();
                    } else {
                        alvoInfo = "Turma ID: " + std::to_string(a.getAlvoId()) + " (Disciplina Desconhecida)";
                    }
                } else {
                     alvoInfo = "Turma ID Desconhecido: " + std::to_string(a.getAlvoId());
                }

            } else if (a.getTipo() == "DISCIPLINA") {
                Disciplina* disciplina = nullptr;
                for (auto &d : _disciplinas) {
                    if (d.getId() == a.getAlvoId()) {
                        disciplina = &d;
                        break;
                    }
                }
                if (disciplina) {
                    alvoInfo = "Disciplina: " + disciplina->getCodigo() + " (" + disciplina->getNome() + ")";
                } else {
                    alvoInfo = "Disciplina ID Desconhecida: " + std::to_string(a.getAlvoId());
                }

            } else if (a.getTipo() == "PROFESSOR") {
                Usuario* professor = nullptr;
                for (auto *u : _usuarios) {
                    if (u->getId() == a.getAlvoId() && (u->getTipo() == "PROFESSOR" || u->getTipo() == "COORDENADOR_DISCIPLINA" || u->getTipo() == "COORDENADOR_DO_CURSO")) {
                        professor = u;
                        break;
                    }
                }
                if (professor) {
                    alvoInfo = "Professor: " + professor->getNome() + " (ID: " + std::to_string(a.getAlvoId()) + ")";
                } else {
                    alvoInfo = "Professor ID Desconhecido: " + std::to_string(a.getAlvoId());
                }

            } else {
                 alvoInfo = "Alvo ID: " + std::to_string(a.getAlvoId());
            }
            
            std::cout << "ID: " << a.getId() << " | Tipo: " << a.getTipo() << " | " << alvoInfo << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
    }
}
// SALVAR
void SistemaAvaliacao::salvarTudo() {
    //salva todos os dados nos arquivos
    arquivo.salvarUsuarios(_usuarios);
    arquivo.salvarDisciplinas(_disciplinas);
    arquivo.salvarTurmas(_turmas);
    arquivo.salvarAvaliacoes(_avaliacoes);
    arquivo.salvarMatriculas(_usuarios);

    std::cout << "Dados salvos em disco (data/)\n";
}