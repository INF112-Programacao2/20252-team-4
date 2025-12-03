# Sistema de Avaliações  
**Projeto Final – Programação 2**

Este projeto implementa um sistema para registro e consulta de avaliações anônimas para as disciplinas do Departamento de Informática da UFV.  
O Coordenador de Curso atua como administrador, sendo o responsável pelo cadastro de usuários, disciplinas e turmas.

---

## Objetivo do Desenvolvimento Sustentável (ODS)

Este projeto está alinhado ao **ODS 4 – Educação de Qualidade**:

**ODS 4 – Educação de Qualidade**  
Assegurar a educação inclusiva e equitativa e de qualidade, e promover oportunidades de aprendizagem ao longo da vida para todos.

O sistema contribui para este objetivo ao permitir avaliações contínuas, imparciais e anônimas, promovendo melhoria da qualidade de ensino por meio de feedback estruturado entre alunos, professores e coordenações.

---

## Sumário
- [Autores](#autores)
- [Descrição Geral](#descrição-geral)  
- [Arquitetura Utilizada](#arquitetura-utilizada)  
- [Users Stories](#users-stories)
- [Cartões CRC](#cartões-crc)
- [Armazenamento de Dados](#armazenamento-de-dados)  
- [Como Compilar e Executar](#como-compilar-e-executar)  

---

## Autores

Projeto desenvolvido por:

- Bianca Solleny Souza Silva - 116235 

- Janaína Aparecida da Silva - 78860

- Júlia Ester Cirino de Oliveira - 120586

- Maria Clara Souza Silva - 120559

---

## Descrição Geral

O Sistema de Avaliação do DPI é um software em C++ que opera via terminal e permite avaliações anônimas de:

- Disciplinas  
- Professores  
- Turmas  

Cada tipo de usuário possui permissões específicas, definidas por herança e polimorfismo.  
O projeto segue os conceitos trabalhados na disciplina de Programação 2 da UFV.

---

## Arquitetura Utilizada

O sistema foi construído utilizando:

- Encapsulamento  
- Herança  
- Polimorfismo por classes derivadas  
- Classes abstratas  
- Modularização em arquivos `.hpp` e `.cpp`  
- Tratamento de exceções  
- Recursividade  
- Persistência via arquivos `.txt`

  [Sumário](#sumário)
---
## Users Stories

### Aluno

**User Stories**  
Como aluno, quero poder avaliar os meus professores e as disciplinas nas quais estou matriculado. Também quero poder ver a média das avaliações do professor e da disciplina, assim como checar o meu histórico de avaliações.

**Critérios de Aceitação**
- Fazer avaliação das disciplinas.  
- Fazer avaliações dos professores.  
- Checar a média da disciplina e do professor.  
- Checar o histórico de avaliações.

---

### Professor

**User Stories**  
Como professor da disciplina, desejo ver as avaliações feitas pelos alunos, desejo avaliar as turmas e checar meu histórico de avaliações.

**Critérios de Aceitação**
- Fazer avaliação das minhas turmas.  
- Visualizar as avaliações em períodos distintos (dia, semana, mês, período).  
- Ter uma nota atribuída a mim de acordo com a disciplina ministrada.  
- Visualizar meu histórico de avaliações.

---

### Coordenador de Disciplina

**User Stories**  
Como Coordenador da disciplina, eu quero acessar as funções de um professor e também poder ver as avaliações da disciplina e dos professores.

**Critérios de Aceitação**
- Visualizar as avaliações feitas pelos alunos para os professores.  
- Visualizar as avaliações feitas pelos professores para a turma.  
- Visualizar as avaliações feitas pelos alunos para a disciplina.  
- Fazer avaliação das minhas turmas.

---

### Coordenador do Curso

**User Stories**  
Como coordenador do curso, desejo cumprir o papel de administrador do sistema, realizando o cadastro de alunos e professores, bem como cadastro de turmas e disciplinas. Também desejo matricular os alunos nas suas respectivas turmas e ver as avaliações gerais.

**Critérios de Aceitação**
- Gerar relatórios das avaliações feitas pelos alunos e professores de forma geral não detalhada (somente avaliações).  
- Gerar relatórios das avaliações feitas pelos alunos e professores de forma geral detalhada (médias e comentários).  
- Matricular aluno em turma.  
- Cadastrar usuário (Aluno ou Professor).  
- Cadastrar turma.  
- Cadastrar disciplina.
  
  [Sumário](#sumário)
---
## Cartões CRC

### Usuário (Classe Mãe)

| **Classe** | Usuário |
|-----------|---------|
| **Responsabilidades** | Armazenar dados básicos (ID, nome, email, senha, tipo). <br> Servir como base para Aluno, Professor e Coordenadores. <br> Fornecer estrutura central para login e menus. |
| **Colaboradores** | SistemaAvaliacao, Aluno, Professor, CoordenadorDisciplina, CoordenadorCurso |

---

### Aluno

| **Classe** | Aluno |
|-----------|--------|
| **Responsabilidades** | Realizar avaliações de disciplinas e professores. <br> Visualizar médias. <br> Consultar histórico de avaliações. |
| **Colaboradores** | Disciplina, Professor, Avaliacao, SistemaAvaliacao |

---

### Professor

| **Classe** | Professor |
|-----------|-----------|
| **Responsabilidades** | Avaliar turmas. <br> Consultar avaliações recebidas. <br> Visualizar histórico. |
| **Colaboradores** | Turma, Disciplina, Avaliacao, SistemaAvaliacao |

---

### Coordenador de Disciplina

| **Classe** | CoordenadorDisciplina |
|-----------|------------------------|
| **Responsabilidades** | Gestão da disciplina. <br> Visualização das avaliações feitas para disciplina, professor e turmas. |
| **Colaboradores** | Disciplina, Turma, Avaliacao, SistemaAvaliacao |

---

### Coordenador do Curso

| **Classe** | CoordenadorCurso |
|-----------|------------------|
| **Responsabilidades** | Administrar o sistema. <br> Cadastrar usuários/disciplina/turma. <br> Matricular alunos. <br> Designar funções para professores. <br> Consultar avaliações gerais. |
| **Colaboradores** | Usuario, Professor, CoordenadorDisciplina, Aluno, Disciplina, Turma, SistemaAvaliacao |

---

### Disciplina

| **Classe** | Disciplina |
|-----------|------------|
| **Responsabilidades** | Armazenar ID, código e nome da disciplina. <br> Associar professores e coordenador da disciplina. <br> Relacionar turmas. |
| **Colaboradores** | Professor, CoordenadorDisciplina, Turma, Avaliacao, SistemaAvaliacao |

---

### Turma

| **Classe** | Turma |
|-----------|--------|
| **Responsabilidades** | Armazenar informações da turma (ID, disciplinaID, professor). <br> Relacionar alunos. <br> Receber avaliações. |
| **Colaboradores** | Disciplina, Professor, Aluno, Avaliacao, SistemaAvaliacao |

---

### Avaliacao

| **Classe** | Avaliacao |
|-----------|-----------|
| **Responsabilidades** | Registrar avaliação anônima. <br> Armazenar alvo, nota, comentário e data. |
| **Colaboradores** | Disciplina, Professor, Turma, SistemaAvaliacao, GerenciadorDados |

---

### Sistema de Avaliação

| **Classe** | SistemaAvaliacao |
|-----------|-------------------|
| **Responsabilidades** | Controlar o fluxo do sistema. <br> Gerenciar usuários, disciplinas, turmas e avaliações. <br> Realizar login, operações de cadastro, designação e matrícula. <br> Buscar e tratar dados. |
| **Colaboradores** | Usuario + subclasses, Disciplina, Turma, Avaliacao, GerenciadorDados |

---

### Login

| **Classe** | Login |
|-----------|--------|
| **Responsabilidades** | Realizar autenticação de usuários. <br> Validar email e senha. <br> Retornar o objeto `Usuario*` correspondente ao login. <br> Centralizar a lógica de busca de usuário pelo email. <br> Tratar erros de autenticação (usuário inexistente, senha incorreta). |
| **Colaboradores** | Usuario, Aluno, Professor, CoordenadorDisciplina, CoordenadorCurso, SistemaAvaliacao, GerenciadorDados, picosha2 |

---

### Gerenciador de Dados

| **Classe** | GerenciadorDados |
|-----------|-------------------|
| **Responsabilidades** | Ler e salvar todos os arquivos `.txt`. <br> Carregar dados na inicialização. <br> Atualizar arquivos após operações. |
| **Colaboradores** | Usuario, Disciplina, Turma, Avaliacao, SistemaAvaliacao |

---

## Armazenamento de Dados

O sistema utiliza arquivos `.txt` para guardar informações, sem uso de banco de dados.  
As avaliações são anônimas e armazenadas de forma serializada, seguindo o padrão:

```
Id;alvoId;tipo;nota;comentario;data
```
Sendo alvoId o Id do objeto que está sendo avaliado.

- Nenhuma informação de identificação pessoal é salva nos arquivos de avaliações.

---

## Como Compilar e Executar

No terminal do Vs code com a extensão do Makefile instalada, o usuário deve inserir:

```
make run
```
E irá compilar e inicializar o programa.

- [Sumário](#sumário)
No terminal do Vs code com a extensão do Makefile instalada, o usuário deve inserir:

```
make run
```
E irá compilar e inicializar o programa.
