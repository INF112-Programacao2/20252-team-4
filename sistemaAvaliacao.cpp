#include "sistemaAvaliacao.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

int lerInteiroComExcecao(const std::string& pergunta) {
    std::string entrada;

    while (true) {
        std::cout << pergunta;
        std::getline(std::cin, entrada);

        try {
            int valor = std::stoi(entrada);

            // Verifica intervalo permitido
            if (valor < 0 || valor > 5) {
                std::cout << "Erro: o valor deve estar entre 0 e 5.\n";
                continue; // volta ao início do loop
            }

            return valor; // valor válido
        }
        catch (const std::invalid_argument&) {
            std::cout << "Erro: digite apenas números inteiros.\n";
        }
    }
}

// carrega tudo do GerenciadorDados
SistemaAvaliacao::SistemaAvaliacao() {
    arquivo.carregarUsuarios(_usuarios);
    _disciplinas = arquivo.carregarDisciplinas();
    _turmas = arquivo.carregarTurmas();
    _avaliacoes = arquivo.carregarAvaliacoes();


    // Se nao existir nenhum coordenador do curso, cria um padrao
    bool temCoord = false;
    for (auto u : _usuarios) if (u->getTipo() == "COORDENADOR_DO_CURSO") { 
        temCoord = true; 
        break; 
    }
    if (!temCoord) {
        // adiciona um administrador padrao
        int proximoId = ProximoIdUsuarios(_usuarios);
        CoordenadorCurso *admin = new CoordenadorCurso(proximoId, "Admin Curso", "admin@ufv.br", "admin123");
        _usuarios.insert(_usuarios.begin(), admin);
    }
}

// libera ponteiros de _usuarios
SistemaAvaliacao::~SistemaAvaliacao() {
    for (auto u : _usuarios) delete u;
}

// gerar novo id para cada tipo

int SistemaAvaliacao::ProximoIdUsuarios(const std::vector<Usuario*> &v) {
    
    int ultimoId = 0;
    for (auto u : v) if (u->getId() > ultimoId) ultimoId = u->getId();
    return ultimoId + 1;
}

int SistemaAvaliacao::ProximoIdDisciplinas(const std::vector<Disciplina> &v) {
    int ultimoId = 0;
    for (auto &d : v) if (d.getId() > ultimoId) ultimoId = d.getId();
    return ultimoId + 1;
}

int SistemaAvaliacao::ProximoIdTurmas(const std::vector<Turma> &v) {
    int ultimoId = 0;
    for (auto &t : v) if (t.getId() > ultimoId) ultimoId = t.getId();
    return ultimoId + 1;
}

int SistemaAvaliacao::ProximoIdAvaliacoes(const std::vector<Avaliacao> &v) {
    int ultimoId = 0;
    for (auto &a : v) if (a.getId() > ultimoId) ultimoId = a.getId();
    return ultimoId + 1;
}

// get
std::vector<Usuario*>& SistemaAvaliacao::getUsuarios() {
    return _usuarios;
}

// CADASTROS

void SistemaAvaliacao::cadastrarUsuario(const int &tipo) {
    if (tipo != 1 && tipo != 2) {
        throw "Tipo de usuario invalido. Escolha 1 (Aluno) ou 2 (Professor).\n";
    }
    std::string nome, email, senha;
    std::cout << "Nome: ";
    std::getline(std::cin >> std::ws, nome); //usamos std::ws para descartar espaco em branco, quebra de linha e tabulacao

    const std::string validarEmail = "@ufv.br";
    const size_t tamanhoEmail = validarEmail.length();

    while (true) {
        std::cout << "email: ";
        
        if (!(std::cin >> email)) {
            std::cerr << "Erro na leitura do email.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        //Verificar formato do email
        if (email.length() < tamanhoEmail || 
            email.substr(email.length() - tamanhoEmail) != validarEmail ||
            email.length() == tamanhoEmail) {
            std::cerr << "ERRO: O e-mail deve ser do dominio '@ufv.br'. Tente novamente.\n";
            continue;
        }

        //Verificar se email e' repetido
        bool emailExiste = false;
        for (const auto* u : _usuarios) {
            if (u->getemail() == email) {
                emailExiste = true;
                break;
            }
        }

        if (emailExiste) {
            std::cerr << "ERRO: Usuario " << email << " ja esta cadastrado no sistema. Tente novamente.\n";
            continue;
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
}

void SistemaAvaliacao::cadastrarDisciplina() {

    std::string codigo, nome;
    int profId, coordId;
    
    //Tratamento de excecao para o codigo da disciplina
    while (true) {
        std::cout << "Codigo da disciplina (ex: INF112): ";
        
        if (!(std::cin >> codigo)) {
            std::cerr << "Erro na leitura do codigo.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            continue;
        }

        if (codigo.length() != 6 || codigo.substr(0, 3) != "INF") {
            std::cerr << "ERRO: O codigo da disciplina deve ter 6 caracteres, começando com 'INF' (em maiusculas) e seguido por tres numeros (ex: INF112). Tente novamente.\n";
            continue;
        }
        
        int numCodigo;
        try {
            std::string numStr = codigo.substr(3); 
            numCodigo = std::stoi(numStr);

            if (numCodigo <= 100 || numCodigo >= 999) {
                std::cerr << "ERRO: Os tres numeros do codigo (depois de 'INF') devem estar entre 100 e 999. Tente novamente.\n";
                continue;
            }
        }
        catch (const std::invalid_argument&) {
            std::cerr << "ERRO: Os caracteres apos 'INF' devem ser numeros validos. Tente novamente.\n";
            continue; 
        } 
        catch (const std::out_of_range&) {
            std::cerr << "ERRO: Numero fora do intervalo. Tente novamente.\n";
            continue; 
        }

        bool codigoExiste = false;
        for (const auto &d : _disciplinas) {
            if (d.getCodigo() == codigo) {
                codigoExiste = true;
                break;
            }
        }

        if (codigoExiste) {
            std::cerr << "ERRO: Codigo de disciplina '" << codigo << "' ja' cadastrado. Tente outro.\n";
            continue; 
        } else {
            break; 
        }
    }
    
    //Tratamento de excecao para o nome de disciplina
    while (true) {
        std::cout << "Nome da disciplina: ";
        std::getline(std::cin >> std::ws, nome);

        if (nome.empty()) {
            std::cerr << "ERRO: O nome da disciplina nao pode ser vazio. Tente novamente.\n";
            continue;
        }
        
        bool nomeExiste = false;
        for (const auto &d : _disciplinas) {
            if (d.getNome() == nome) {
                nomeExiste = true;
                break;
            }
        }

        if (nomeExiste) {
            std::cerr << "ERRO: Nome de disciplina '" << nome << "' ja' cadastrado. Tente outro.\n";
            continue;
        } else {
            break;
        }
    }

    std::cout << "Professores disponiveis para coordenador:\n";
    for (auto u : _usuarios) {
        std::string tipo = u->getTipo();
        if (tipo == "PROFESSOR" || tipo == "COORDENADOR_DO_CURSO") {
            std::cout << u->getId() << " - " << u->getNome() << '\n';
        }
    }

    std::cout << "Escolha o ID do professor da disciplina: ";
    std::cin >> coordId;

    int id = ProximoIdDisciplinas(_disciplinas);
    _disciplinas.emplace_back(id, codigo, nome, profId, coordId);
    std::cout << "Disciplina cadastrada (ID=" << id << ")\n";
}

void SistemaAvaliacao::cadastrarTurma() {

    int disciplinaId;
    std::string codigoTurma;
    int professorId;

    if (_disciplinas.empty()) {
        throw "Nao e' possivel cadastrar turmas. Nao existem disciplinas cadastradas. ";
    }
    std::cout << "Disciplinas disponiveis:\n";


    for (auto &d : _disciplinas) 
        std::cout << d.getId() << " - " << d.getCodigo() << " " << d.getNome() << '\n';

    std::cout << "Escolha ID da disciplina: ";
    std::cin >> disciplinaId;

    std::cout << "Codigo da turma (ex: 1): ";
    std::cin >> codigoTurma;

    std::cout << "Professores disponiveis:\n";
    for (auto u : _usuarios) if (u->getTipo() == "PROFESSOR" || u->getTipo() == "COORDENADOR_DISCIPLINA" || u->getTipo() == "COORDENADOR_DO_CURSO")
        std::cout << u->getId() << " - " << u->getNome() << '\n';
    std::cout << "Escolha ID do professor para a turma: ";
    std::cin >> professorId;

    int id = ProximoIdTurmas(_turmas);
    _turmas.emplace_back(id, disciplinaId, codigoTurma, professorId);
    std::cout << "Turma cadastrada (ID=" << id << ")\n";
}

// AVALIACOES
void SistemaAvaliacao::avaliarDisciplina(Usuario* u) {
    std::vector<Turma*> turmasemaildas = u->getMinhasDisciplinas();

    if (turmasemaildas.empty()) {
        throw "Voce nao esta matriculado em nenhuma disciplina. Nenhuma disciplina disponivel para avaliacao.\n";
    }

    std::vector<int> disciplinasIds;
    for (const auto* t : turmasemaildas) {
        disciplinasIds.push_back(t->getDisciplinaId()); // Turma sabe sua Disciplina
    }

    // 3. Remover duplicatas (se o aluno estiver em várias turmas da mesma disciplina)
    //std::sort(disciplinasIds.begin(), disciplinasIds.end());
    //disciplinasIds.erase(std::unique(disciplinasIds.begin(), disciplinasIds.end()), disciplinasIds.end());

    // 4. Listar apenas as disciplinas que o aluno está cursando
    std::cout << "Disciplinas emaildas disponiveis para avaliacao:\n";
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

    int idDisc;
    bool idValido = false;
    do {

        std::cout << "Escolha ID da disciplina: ";
        std::cin >> idDisc;

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

    int nota[5];
    std::cout << "Como voce avalia a disciplina? (0-5)\n";
    std::cout << "Sendo 0 discordo totalmente, e 5 concordo plenamente.\n";

    nota[0] = lerInteiroComExcecao("Em relacao a ementa: ");
    nota[1] = lerInteiroComExcecao("\nVoce considera o conteudo da disciplina relevante para sua formacao?: ");
    nota[2] = lerInteiroComExcecao("\nOs meios de avaliar a aprendizagem foram eficazes? (ex.: provas e seminarios): ");
    nota[3] = lerInteiroComExcecao("\nO conteudo planejado foi compativel com a carga horaria da disciplina?: ");
    nota[4] = lerInteiroComExcecao("\nA disciplina complementa/agrega conhecimento para o restante do curso?: ");

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string comentario;
    std::cout << "\nDeixe aqui elogios, sugestoes ou cri'ticas para aprimoramento da disciplina: ";
    std::getline(std::cin >> std::ws, comentario);

    double mediaNotas = 0.0;
    for (int i=0; i<5; i++)
        mediaNotas += nota[i];
    mediaNotas /= 5.0;
    int id = ProximoIdAvaliacoes(_avaliacoes);
    _avaliacoes.emplace_back(id, idDisc, "DISCIPLINA", mediaNotas, comentario, getDataAtual());
    std::cout << "Avaliacao de disciplina registrada.\n";
}

void SistemaAvaliacao::avaliarProfessor(Usuario* u) {
    std::vector<Turma*> turmasemaildas = u->getMinhasDisciplinas();

    if (turmasemaildas.empty()) {
        throw "Voce nao esta' matriculado em nenhuma turma. Nenhum professor disponi'vel para avaliacao.\n";
    }

    std::vector<int> professoresIds;
    for (const auto* t : turmasemaildas) {
        int profId = t->getProfessorId(); // Turma sabe quem é o professor
        if (profId > 0) { 
            professoresIds.push_back(profId); 
        }
    }

    std::cout << "Professores disponiveis para avaliacao:\n";
    std::vector<Usuario*> professoresDisponiveis;
    for (int profId : professoresIds) {
        for (auto &usuario : _usuarios) {
            if (usuario->getId() == profId && usuario->getTipo() == "PROFESSOR") {
                std::cout << usuario->getId() << " - " << usuario->getNome() << '\n';
                professoresDisponiveis.push_back(usuario);
                break;
            }
        }
    }

    bool idValido = false;
    int profId;
    do {
        std::cout << "Escolha ID do professor: "; 
        std::cin >> profId;

        for(const auto* p : professoresDisponiveis) {
            if (p->getId() == profId) {
                idValido = true;
                break;
            }
        }
        if (!idValido) {
            std::cerr << "ID de professor invalido. Tente novamente\n";
        }
    } while (!idValido);

    int nota[5]; 
    std::cout << "Como voce avalia o professor? (0-5)\n";
    std::cout << "Sendo 0 discordo totalmente, e 5 concordo plenamente.\n";

    nota[0] = lerInteiroComExcecao("O professor forneceu feedback das avaliacoes?: ");
    nota[1] = lerInteiroComExcecao("\nA metodologia de ensino do professor favoreceu a aprendizagem?: ");
    nota[2] = lerInteiroComExcecao("\nO professor incentivou os alunos a participarem das aulas?: ");
    nota[3] = lerInteiroComExcecao("\nO professor demonstrou domi'nio no conteu'do da disciplina?: ");
    nota[4] = lerInteiroComExcecao("\nA dida'tica do professor foi objetiva e clara?: ");

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string comentario; 
    std::cout << "\nDeixe aqui elogios, sugestoes ou cri'ticas para aprimoramento da metodologia utilizada: ";
    std::getline(std::cin >> std::ws, comentario);

    double mediaNotas = 0.0;
    for (int i=0; i<5; i++)
        mediaNotas += nota[i];
    mediaNotas /= 5.0;
    int id = ProximoIdAvaliacoes(_avaliacoes);

    _avaliacoes.emplace_back(id, profId, "PROFESSOR", mediaNotas, comentario, getDataAtual());
    std::cout << "Avaliacao de professor registrada.\n";
}

void SistemaAvaliacao::avaliarTurma(Usuario* u) {

    if (_turmas.empty()) { 
        throw "Nenhuma turma cadastrada no sistema.\n";
    }

    // 2. Filtra as turmas ministradas pelo professor logado
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

    // 3. Lista as turmas filtradas
    std::cout << "Suas Turmas (Turmas que voce ministra):\n";
    for (auto &t : turmasMinistradas) {
        std::string discInfo = "Disciplina Desconhecida";
        // Busca o código da disciplina para melhor visualização
        for (auto &d : _disciplinas) {
            if (d.getId() == t->getDisciplinaId()) {
                discInfo = d.getCodigo();
                break;
            }
        }
        std::cout << t->getId() << " - Disciplina: " << discInfo << " | Turma: " << t->getCodigoTurma() << '\n';
    }


    int turmaId;
    bool idValido = false;
    do {
        std::cout << "Escolha ID da turma: "; 
        std::cin >> turmaId;

        for(const auto* t : turmasMinistradas) {
            if (t->getId() == turmaId) {
                idValido = true;
                break;
            }
        }

        if (!idValido) {
            std::cerr << "\nID de turma inva'lido ou voce nao ministra esta turma.\n";
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

        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string comentario; 
        std::cout << "\nComentario: "; 
        std::getline(std::cin >> std::ws, comentario);

        double mediaNotas = 0;
        for (int i=0; i<5; i++)
            mediaNotas += nota[i];
        mediaNotas /= 5.0;
        int id = ProximoIdAvaliacoes(_avaliacoes);
        _avaliacoes.emplace_back(id, turmaId, "TURMA", mediaNotas, comentario, getDataAtual());
        std::cout << "Avaliacao de turma registrada.\n";
}

//LISTAGEM
void SistemaAvaliacao::listarAvaliacoes(const std::string &tipo) {

    if (tipo == "GERAL") {
        std::cout << " Avaliacoes disponiveis : \n";

        for (auto &a : _avaliacoes) {
            std::cout << "ID: " << a.getId() << " | Tipo: " << a.getTipo() << " | Alvo: " << a.getAlvoId() << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
        return;
    }

    // filtrar por tipo
    std::cout << " Avaliacoes do tipo " << tipo << std::endl;
    for (auto &a : _avaliacoes) {
        if (a.getTipo() == tipo) {
            std::cout << "ID: " << a.getId() << " |  Disciplina: "<< a.get << "- Turma: " << a.getAlvoId() << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
    }
}

// SALVAR
void SistemaAvaliacao::salvarTudo() {
    // NOTA: salvarUsuarios precisa de getSenha() no header para escrever senhas corretas.
    arquivo.salvarUsuarios(_usuarios);
    arquivo.salvarDisciplinas(_disciplinas);
    arquivo.salvarTurmas(_turmas);
    arquivo.salvarAvaliacoes(_avaliacoes);
    std::cout << "Dados salvos em disco (data/)\n";
}