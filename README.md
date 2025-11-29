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
- [Funcionalidades](#funcionalidades)  
- [Arquitetura Utilizada](#arquitetura-utilizada)  
- [Estrutura do Projeto](#estrutura-do-projeto)  
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

O Sistema Universitário de Avaliações é um software em C++ que opera via terminal e permite avaliações anônimas de:

- Disciplinas  
- Professores  
- Turmas  

Cada tipo de usuário possui permissões específicas, definidas por herança e polimorfismo.  
O projeto segue os conceitos trabalhados na disciplina de Programação 2 da UFV.

---

## Funcionalidades

### Aluno
- Avaliar disciplinas  
- Avaliar professores  
- Consultar suas avaliações  
- Ver médias das disciplinas

### Professor
- Avaliar turmas  
- Ver avaliações recebidas  
- Acompanhar desempenho das suas turmas

### Coordenador de Disciplina
- Acessar avaliações da disciplina  
- Consultar médias por turma  
- Gerar relatórios

### Coordenador de Curso (Administrador)
- Cadastrar usuários  
- Cadastrar disciplinas  
- Cadastrar turmas  
- Acessar estatísticas gerais do curso  

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

---

## Armazenamento de Dados

O sistema utiliza arquivos `.txt` para guardar informações, sem uso de banco de dados.  
As avaliações são anônimas e armazenadas de forma serializada, seguindo o padrão:

```
alvoId;tipo;nota;comentario;data
```

Nenhuma informação de identificação pessoal é salva nos arquivos de avaliações.
