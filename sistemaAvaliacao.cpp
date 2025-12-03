#include "sistemaAvaliacao.hpp"
#include <iostream>
#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>
#include <filesystem>


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
        CoordenadorCurso *admin = new CoordenadorCurso(proximoId, "Admin Curso", "admin@ufv.br", "admin123");
        _usuarios.insert(_usuarios.begin(), admin);
    }
}

Disciplina* SistemaAvaliacao::getDisciplinaPorID(int id) {
    for (auto &d : _disciplinas) {
        if (d.getId() == id) return &d;
    }
    return nullptr;
}

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

    arquivo.salvarUsuarios(_usuarios);
}

void SistemaAvaliacao::cadastrarDisciplina() {

    std::string codigo, nome;
    int coordenadorId;
    
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

            if (numCodigo < 100 || numCodigo > 999) {
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
    // LISTAGEM DE PROFESSORES PARA ESCOLHA DO COORDENADOR
    std::vector<Usuario*> professoresDisponiveis;
    for (auto u : _usuarios) {
        std::string tipo = u->getTipo();
        // Permite coordenadores de curso, disciplina ou professor comum como coordenador
        if (tipo == "PROFESSOR" || tipo == "COORDENADOR_DISCIPLINA" || tipo == "COORDENADOR_DO_CURSO") {
            std::cout << u->getId() << " - " << u->getNome() << " (" << tipo << ")\n";
            professoresDisponiveis.push_back(u);
        }
    }

    Usuario* profCoordenador = nullptr;
    while(profCoordenador == nullptr) {
        std::cout << "Escolha o ID do professor para COORDENADOR da disciplina: ";
        if (!(std::cin >> coordenadorId)) {
            std::cerr << "ERRO: Entrada inva'lida. Tente novamente.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        for (auto p : professoresDisponiveis) {
            if (p->getId() == coordenadorId) {
                profCoordenador = p;
                break;
            }
        }
        if (profCoordenador == nullptr) {
            std::cerr << "ERRO: ID de professor inva'lido. Tente novamente.\n";
        }
    }

    // 1. PROMOÇÃO AUTOMÁTICA: se for um PROFESSOR comum, é promovido.
    if (profCoordenador->getTipo() == "PROFESSOR") {
        profCoordenador->setTipo("COORDENADOR_DISCIPLINA"); // AGORA FUNCIONARÁ
        std::cout << "\nProfessor " << profCoordenador->getNome() << " promovido para COORDENADOR_DISCIPLINA.\n";
    }

    int novaDiscId = ProximoIdDisciplinas(_disciplinas);
    int profIdResponsavel = coordenadorId; // O coordenador é o professor responsável pela disciplina
    
    // 2. CADASTRO DA DISCIPLINA
    _disciplinas.emplace_back(novaDiscId, codigo, nome, profIdResponsavel, coordenadorId);
    std::cout << "Disciplina cadastrada (ID=" << novaDiscId << ")\n";
    
    // 3. CRIAÇÃO AUTOMÁTICA DA TURMA 1
    int novaTurmaId = ProximoIdTurmas(_turmas);
    std::string codigoTurma = "1";
    int professorTurmaId = coordenadorId; // O coordenador é o professor da Turma 1

    _turmas.emplace_back(novaTurmaId, novaDiscId, codigoTurma, professorTurmaId);
    std::cout << "Turma 1 automatica cadastrada (ID=" << novaTurmaId << "). Professor: " << profCoordenador->getNome() << ".\n";

    arquivo.salvarDisciplinas(_disciplinas);
}

void SistemaAvaliacao::cadastrarTurma() {

    try {
        int disciplinaId, professorId;
        std::string codigoTurma;

        if (_disciplinas.empty()) {
            throw "Nao e' possivel cadastrar turmas. Nao existem disciplinas cadastradas. ";
        }

        std::cout << "Disciplinas disponiveis:\n";
            for (auto &d : _disciplinas) 
            std::cout << d.getId() << " - " << d.getCodigo() << " " << d.getNome() << '\n';

        std::cout << "Escolha ID da disciplina: ";
        std::cin >> disciplinaId;

        bool codigoJaExiste;
        do {
            std::cout << "Codigo da turma (ex: 1): ";
            std::cin >> codigoTurma;

            codigoJaExiste = false;

            for (const auto& t : _turmas) {
                if (t.getDisciplinaId() == disciplinaId &&
                    t.getCodigoTurma() == codigoTurma)
                {
                    codigoJaExiste = true;
                    std::cout << "Erro: ja existe uma turma com esse codigo. Tente outro: \n";
                    break;
                }
            }

        } while (codigoJaExiste);

        std::cout << "Professores disponiveis:\n";
            for (auto u : _usuarios) if (u->getTipo() == "PROFESSOR" || u->getTipo() == "COORDENADOR_DISCIPLINA")
            std::cout << u->getId() << " - " << u->getNome() << '\n';
            std::cout << "Escolha ID do professor para a turma: ";
            std::cin >> professorId;

        int id = ProximoIdTurmas(_turmas);
        _turmas.emplace_back(id, disciplinaId, codigoTurma, professorId);
        std::cout << "Turma cadastrada (ID=" << id << ")\n";
    }
        catch (const std::runtime_error& e) {
            std::cerr << e.what() << '\n';
        }
        catch (const char* msg) {
            std::cerr << msg << '\n';
        }
    arquivo.salvarTurmas(_turmas);
}


void SistemaAvaliacao::matricularAluno() {

    // 1. Verificar se há alunos e turmas
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


    // 2. Escolher o aluno
    std::cout << "\nAlunos disponi'veis:\n";
    for(const auto* a : alunos) {
        std::cout << "ID: " << a->getId() << " - " << a->getNome() << " (" << a->getemail() << ")\n";
    }

    int alunoId;
    Usuario* alunoSelecionado = nullptr;
    while(alunoSelecionado == nullptr) {
        std::cout << "Escolha o ID do aluno para matricular: ";
        if (!(std::cin >> alunoId)) {
            std::cerr << "ERRO: Entrada inva'lida. Tente novamente.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        for(auto a : alunos) {
            if (a->getId() == alunoId) {
                alunoSelecionado = a;
                break;
            }
        }
        if (alunoSelecionado == nullptr) {
            std::cerr << "ERRO: ID de aluno inva'lido ou aluno na'o e' do tipo ALUNO. Tente novamente.\n";
        }
    }
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    // 3. Escolher a disciplina da turma
    int disciplinaIdEscolhida = -1;

    // lista disciplinas
    std::cout << "\nDisciplinas disponi'veis:\n";
    for (const auto &d : _disciplinas) {
        std::cout << d.getId() << " - " << d.getCodigo() << " - " << d.getNome() << '\n';
    }

    bool discValida = false;
    while (!discValida) {
        std::cout << "Escolha o ID da disciplina para matricular o aluno: ";
        if (!(std::cin >> disciplinaIdEscolhida)) {
            std::cerr << "ERRO: Entrada inva'lida. Tente novamente.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

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

    std::vector<Turma*> turmasDaDisciplina;
    std::cout << "\nTurmas disponi'veis para essa disciplina:\n";
    for (auto &t : _turmas) {
        if (t.getDisciplinaId() == disciplinaIdEscolhida) {
            std::cout << t.getId()
                    << " - Turma: " << t.getCodigoTurma()
                    << " (Disciplina ID=" << t.getDisciplinaId() << ")\n";
            turmasDaDisciplina.push_back(&t);
        }
    }

    if (turmasDaDisciplina.empty()) {
        std::cerr << "ERRO: Nao ha turmas cadastradas para essa disciplina.\n";
        return;
    }

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
        
        // 4. Checar se o aluno já está matriculado nessa turma
        bool jaMatriculado = false;
        for (const auto* t : alunoSelecionado->getMinhasDisciplinas()) {
            if (t->getId() == turmaSelecionada->getId()) {
                jaMatriculado = true;
                break;
            }
        }

        if (jaMatriculado) {
            std::cout << "ERRO: O aluno " << alunoSelecionado->getNome() << " ja' esta' matriculado nesta turma.\n";
            return;
        }

        int disciplinaDaTurmaSelecionada = turmaSelecionada->getDisciplinaId();

        // 4.1 Checar se o aluno já está matriculado em outra turma
        bool jaMatriculadoNaDisciplina = false;

        for (const auto* t : alunoSelecionado->getMinhasDisciplinas()) {
            if (t->getDisciplinaId() == disciplinaDaTurmaSelecionada) {
                jaMatriculadoNaDisciplina = true;
                break;
            }
        }

        if (jaMatriculadoNaDisciplina) {
            std::cout << "\nERRO: O aluno "
                    << alunoSelecionado->getNome()
                    << " ja' esta' matriculado em outra turma dessa mesma disciplina.\n";
            std::cout << "Escolha uma turma de outra disciplina.\n";
            return;
        }


        // 5. Matricular o aluno (adicionar a turma ao vetor _minhasDisciplinas do aluno)
        alunoSelecionado->addTurma(turmaSelecionada);

        std::cout << "\nAluno " << alunoSelecionado->getNome() << " matriculado com sucesso na Turma " << turmaSelecionada->getCodigoTurma() << ".\n";

        arquivo.salvarMatriculas(_usuarios);
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

    arquivo.salvarAvaliacoes(_avaliacoes);
}

void SistemaAvaliacao::avaliarProfessor(Usuario* u) {
    std::vector<Turma*> turmasMatriculadas = u->getMinhasDisciplinas();

    if (turmasMatriculadas.empty()) {
        throw "Voce nao esta' matriculado em nenhuma turma. Nenhum professor disponi'vel para avaliacao.\n";
    }

    // Usaremos um mapa para agrupar as disciplinas por Professor ID.
    // Map: [Professor ID] -> Vector of {Disciplina ID, Disciplina Código}
    std::map<int, std::vector<std::pair<int, std::string>>> professoresDisciplinas; 

    for (const auto* t : turmasMatriculadas) {
        int profId = t->getProfessorId();
        int discId = t->getDisciplinaId();
        Disciplina* disc = getDisciplinaPorID(discId);

        if (profId > 0 && disc) { 
            bool jaAdicionado = false;
            // Verifica se a disciplina já foi adicionada para este professor (para evitar duplicatas)
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

    // 1. Listar professores disponíveis para avaliação
    std::cout << "Professores disponiveis para avaliacao:\n";
    std::map<int, Usuario*> listaProfessores;

    for (auto const& [profId, discs] : professoresDisciplinas) {
        Usuario* usuario = getUsuarioPorID(profId);
        if (usuario && (usuario->getTipo() == "PROFESSOR" || usuario->getTipo() == "COORDENADOR_DISCIPLINA" || usuario->getTipo() == "COORDENADOR_DO_CURSO")) {
            std::cout << usuario->getId() << " - " << usuario->getNome() << '\n';
            listaProfessores[profId] = usuario;
        }
    }
    
    // 2. Escolher o professor
    bool idProfValido = false;
    int profId;
    do {
        std::cout << "Escolha ID do professor: "; 
        std::cin >> profId;

        if (listaProfessores.count(profId)) {
            idProfValido = true;
        } else {
            std::cerr << "ID de professor invalido. Tente novamente\n";
        }
    } while (!idProfValido);
    
    // 3. Mostrar disciplinas disponíveis para avaliação para o professor escolhido
    std::cout << "\nDisciplinas em que " << listaProfessores[profId]->getNome() << " leciona (Escolha para qual avaliar):\n";
    
    const auto& disciplinasDoProfessor = professoresDisciplinas[profId];
    for (const auto& pair : disciplinasDoProfessor) {
        std::cout << pair.first << " - " << pair.second << '\n';
    }
    
    // 4. Escolher a disciplina
    bool idDiscValido = false;
    int discId;
    std::string codigoDisciplina;
    do {
        std::cout << "Escolha ID da disciplina para avaliar: ";
        std::cin >> discId;

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
    
    // O professor será avaliado, mas a disciplina será mencionada no prompt.
    std::cout << "\nAvaliacao para o Professor " << listaProfessores[profId]->getNome() << " na disciplina " << codigoDisciplina << ".\n";


    // Continua com a coleta de notas (Mesmo código)
    int nota[5]; 
    // ... [Resto do código para coletar notas e comentário] ...
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

    // O alvo da avaliação (profId) permanece, mas agora foi feita após a escolha da disciplina.
    _avaliacoes.emplace_back(id, profId, "PROFESSOR", mediaNotas, comentario, getDataAtual());
    std::cout << "Avaliacao de professor registrada.\n";

    arquivo.salvarAvaliacoes(_avaliacoes);
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

        arquivo.salvarAvaliacoes(_avaliacoes);
}

//LISTAGEM
// Implementação da listagem e filtragem avançada para Professores/Coordenadores (Pontos 1 e 2)
void SistemaAvaliacao::visualizarAvaliacoesProfessor(Usuario* u) {
    std::string tipoUsuario = u->getTipo();
    int userId = u->getId();

    std::vector<int> discIdsCoordenadas;
    if (tipoUsuario == "COORDENADOR_DISCIPLINA") {
        for (const auto& d : _disciplinas) {
            if (d.getCoordenadorDiscId() == userId) {
                discIdsCoordenadas.push_back(d.getId());
            }
        }
    }

    std::cout << "\n===== VISUALIZACAO DE AVALIACOES (" << (tipoUsuario == "COORDENADOR_DISCIPLINA" ? "COORDENADOR" : "PROFESSOR") << ") =====\n";
    std::cout << std::fixed << std::setprecision(2);

    int count = 0;
    for (const auto& a : _avaliacoes) {
        bool show = false;
        std::string alvoInfo = "ID: " + std::to_string(a.getAlvoId());

        if (tipoUsuario == "COORDENADOR_DISCIPLINA") {
            // Ponto 1: Professor Coordenador (vê tudo envolvido com sua disciplina)
            
            if (a.getTipo() == "DISCIPLINA") {
                if (std::find(discIdsCoordenadas.begin(), discIdsCoordenadas.end(), a.getAlvoId()) != discIdsCoordenadas.end()) {
                    show = true;
                }
            } else if (a.getTipo() == "PROFESSOR") {
                // Vê avaliações de professores que lecionam nas disciplinas que coordena
                for (const auto& dId : discIdsCoordenadas) {
                    Disciplina* d = getDisciplinaPorID(dId);
                    if (d && d->getProfessorId() == a.getAlvoId()) {
                        show = true;
                        break;
                    }
                }
            } else if (a.getTipo() == "TURMA") {
                Turma* t = getTurmaPorID(a.getAlvoId());
                if (t && std::find(discIdsCoordenadas.begin(), discIdsCoordenadas.end(), t->getDisciplinaId()) != discIdsCoordenadas.end()) {
                    show = true;
                }
            }

        } else if (tipoUsuario == "PROFESSOR") {
            // Ponto 2: Professor Comum (vê avaliações destinadas a ele ou à sua disciplina)
            
            if (a.getTipo() == "PROFESSOR" && a.getAlvoId() == userId) {
                show = true; // Avaliações destinadas a ele
            } else if (a.getTipo() == "DISCIPLINA") {
                // Verifica se ele é o professor principal de alguma turma dessa disciplina
                bool leciona = false;
                for (const auto& t : _turmas) {
                    if (t.getDisciplinaId() == a.getAlvoId() && t.getProfessorId() == userId) {
                        leciona = true;
                        break;
                    }
                }
                if (leciona) {
                    show = true; // Avaliações destinadas à disciplina que ele leciona
                }
            }
        }
        
        // Se a avaliação deve ser exibida, formata a informação do alvo
        if (show) {
            count++;
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
            
            std::cout << "ID: " << a.getId() << " | Tipo: " << a.getTipo() << " | Alvo: " << alvoInfo << " | Nota: " << a.getNota() << " | \"" << a.getComentario() << "\"\n";
        }
    }
    if (count == 0) {
        std::cout << "Nenhuma avaliacao encontrada com base nas suas credenciais.\n";
    }
}

// Implementação do cálculo de médias para o Aluno (Ponto 3)
void SistemaAvaliacao::visualizarMediasAluno(Usuario* u) {
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
    std::map<int, std::pair<std::string, std::vector<double>>> mediasPorProfessor; // ID -> {Nome/Codigo, Lista de Notas}
    std::map<int, std::pair<std::string, std::vector<double>>> mediasPorDisciplina; // ID -> {Nome/Codigo, Lista de Notas}

    for (const auto* turma : turmasAluno) {
        int profId = turma->getProfessorId();
        int discId = turma->getDisciplinaId();
        Disciplina* disc = getDisciplinaPorID(discId);
        Usuario* prof = getUsuarioPorID(profId);

        // Processar Avaliações de PROFESSOR
        if (prof) {
            if (mediasPorProfessor.find(profId) == mediasPorProfessor.end()) {
                mediasPorProfessor[profId].first = prof->getNome();
            }
        }

        // Processar Avaliações de DISCIPLINA
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

    std::cout << "\n===== SUAS MEDIAS DE AVALIAÇÃO (ALUNO) =====\n";
    std::cout << std::fixed << std::setprecision(2);

    // Exibir Médias de Professor
    std::cout << "\n--- Media de Avaliacoes de Professores ---\n";
    for (const auto& pair : mediasPorProfessor) {
        if (!pair.second.second.empty()) {
            double media = std::accumulate(pair.second.second.begin(), pair.second.second.end(), 0.0) / pair.second.second.size();
            std::cout << "Professor: " << pair.second.first << " | Media: " << media << '\n';
        } else {
            std::cout << "Professor: " << pair.second.first << " | Media: N/A (Sem avaliacoes)\n";
        }
    }
    
    // Exibir Médias de Disciplina (Opcional, mas útil para o aluno)
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

// Implementação do Relatório Geral para o Coordenador de Curso (Ponto 4)
void SistemaAvaliacao::relatorioGeralCoordenador() {
    std::cout << "\n===== RELATORIO GERAL (COORDENADOR DE CURSO) =====\n";
    std::cout << std::fixed << std::setprecision(2);

    // Estrutura para armazenar todas as notas e comentários agrupados
    std::map<std::string, std::map<int, std::pair<std::string, std::vector<std::pair<double, std::string>>>>> relatorio; 
    // Tipo -> ID Alvo -> {Nome Alvo, Lista de {Nota, Comentario}}

    // Mapeamento de IDs para Nomes/Códigos para facilitar a exibição
    auto mapTargetInfo = [&](const std::string& tipo, int id) -> std::string {
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
        return "Alvo ID: " + std::to_string(id);
    };

    // 1. Coletar e agrupar todas as avaliações
    for (const auto& a : _avaliacoes) {
        std::string alvoNome = mapTargetInfo(a.getTipo(), a.getAlvoId());
        
        relatorio[a.getTipo()][a.getAlvoId()].first = alvoNome;
        relatorio[a.getTipo()][a.getAlvoId()].second.push_back({a.getNota(), a.getComentario()});
    }

    // 2. Exibir o Relatório
    for (const auto& tipoPair : relatorio) {
        std::cout << "\n--- Avaliacoes do Tipo: " << tipoPair.first << " ---\n";
        
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

void SistemaAvaliacao::listarAvaliacoes(const std::string &tipo) {

    if (tipo == "GERAL") {
        std::cout << " Avaliacoes disponiveis : \n";

        for (auto &a : _avaliacoes) {
            // Lógica para identificar e formatar as informações do alvo
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
    // NOTA: salvarUsuarios precisa de getSenha() no header para escrever senhas corretas.
    arquivo.salvarUsuarios(_usuarios);
    arquivo.salvarDisciplinas(_disciplinas);
    arquivo.salvarTurmas(_turmas);
    arquivo.salvarAvaliacoes(_avaliacoes);
    arquivo.salvarMatriculas(_usuarios);

    std::cout << "Dados salvos em disco (data/)\n";
}