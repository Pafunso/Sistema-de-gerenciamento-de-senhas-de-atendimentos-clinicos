# Sistema de Gerenciamento de Senhas de Atendimento Clínico

Aplicação em linguagem C desenvolvida para informatizar a triagem, classificação de prioridades e controle de filas de atendimento em uma clínica de especialidades médicas.

## Objetivo
Organizar o fluxo de pacientes por meio de filas separadas por nível de prioridade (**Urgente**, **Preferencial** e **Normal**), garantindo ordem de chegada (FIFO) dentro de cada categoria e exibindo um painel textual atualizado de chamadas.

## Funcionalidades
- **Cadastro e emissão automática de senhas:** coleta gênero, idade e condições de prioridade (gestante, lactante, necessidade especial e obesidade) com validação de entradas.
- **Classificação automática:**
  - **Urgente (`U001`, `U002`, ...):** pacientes com mais de 80 anos.
  - **Preferencial (`P001`, `P002`, ...):** gestantes, lactantes, pessoas com necessidades especiais, obesas, mulheres com 60+ anos e homens com 65+ anos.
  - **Normal (`N001`, `N002`, ...):** demais pacientes.
- **Controle de atendimento:** consulta da próxima senha, chamada respeitando a precedência (`Urgente > Preferencial > Normal`), controle de guichê ocupado e finalização do atendimento sem retorno à fila.
- **Painel de chamada:** exibe a senha em atendimento, categoria, próximas senhas na fila e total de pacientes aguardando por categoria.

## Compilação e Execução

Certifique-se de ter o compilador `gcc` instalado.

## Exemplos de aplicação

# Exemplo 1:

Opcao: 1

-- Novo Cadastro --
Genero (M/F/O): M
Idade: 28
Necessidade especial? (S/N): N
Obesidade? (S/N): N

Senha gerada: N001 (Normal)

Opcao: 1

-- Novo Cadastro --
Genero (M/F/O): F
Idade: 25
Gestante? (S/N): S
Lactante? (S/N): N
Necessidade especial? (S/N): N
Obesidade? (S/N): N

Senha gerada: P001 (Preferencial)

Opcao: 1

-- Novo Cadastro --
Genero (M/F/O): M
Idade: 67
Necessidade especial? (S/N): N
Obesidade? (S/N): N

Senha gerada: P002 (Preferencial)

Opcao: 1

-- Novo Cadastro --
Genero (M/F/O): F
Idade: 84
Gestante? (S/N): N
Lactante? (S/N): N
Necessidade especial? (S/N): N
Obesidade? (S/N): N

Senha gerada: U001 (Urgente)

# Exemplo 2:

Opcao: 2

Proxima senha: U001 (Urgente)

Opcao: 3

Chamando senha: U001 (Urgente)

Opcao: 5

----- PAINEL DE CHAMADA -----
Em atendimento: U001 (Urgente)
Proximas senhas: P001 P002 N001 
Em espera -> Urgente: 0 | Preferencial: 2 | Normal: 1
-----------------------------

# Exemplo 3:

Opcao: 3

Ja existe um atendimento em andamento (U001 - Urgente).
Finalize o atendimento atual antes de chamar outro paciente.

Opcao: 4

Atendimento da senha U001 finalizado.

Opcao: 3

Chamando senha: P001 (Preferencial)

Opcao: 5

----- PAINEL DE CHAMADA -----
Em atendimento: P001 (Preferencial)
Proximas senhas: P002 N001 
Em espera -> Urgente: 0 | Preferencial: 1 | Normal: 1
-----------------------------

```bash
gcc -Wall -Wextra -o sistema_senhas main.c


