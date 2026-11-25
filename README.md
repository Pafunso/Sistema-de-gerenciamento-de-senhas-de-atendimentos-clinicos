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
## Funções implementadas

Abaixo estão as principais funções organizadas no código-fonte (`main.c`) para gerenciar o ciclo de vida das senhas e o atendimento na clínica:

### 1. Gerenciamento de filas e prioridades
- `inicializar_filas(Fila *urgente, Fila *preferencial, Fila *normal)`
  Inicializa as estruturas das filas de atendimento (ponteiros/índices, contadores e numeração inicial de senhas), garantindo que comecem vazias antes do início do expediente.
- `gerar_senha(Fila *fila, TipoPrioridade prioridade)`
  Gera o código sequencial da senha conforme a categoria (`U` para Urgente, `P` para Preferencial e `N` para Normal) e insere o registro na fila correspondente respeitando a ordem de chegada (FIFO).
- `esta_vazia(const Fila *fila)`
  Verifica se uma fila específica possui pacientes aguardando atendimento, evitando chamadas inválidas ou *underflow* na estrutura.

### 2. Chamada e atendimento de pacientes
- `chamar_proxima_senha(Fila *urgente, Fila *preferencial, Fila *normal, int guiche)`
  Seleciona o próximo paciente seguindo a precedência de atendimento (`Urgente` -> `Preferencial` -> `Normal`) e a ordem FIFO dentro de cada categoria, removendo a senha da fila de espera e vinculando-a ao `guiche` informado.
- `finalizar_atendimento(Atendimento *atual)`
  Encerra o atendimento ativo no guichê, registra o encerramento da senha e libera o posto para a próxima chamada.

### 3. Visualização e painel de chamadas
- `exibir_painel(const Painel *painel)`
  Exibe no terminal o estado atual do painel de chamadas, destacando a última senha chamada, o guichê de destino e a quantidade de pacientes aguardando por prioridade.
- `listar_filas(const Fila *urgente, const Fila *preferencial, const Fila *normal)`
  Mostra as senhas pendentes em cada fila para conferência operacional da recepção.

### 4. Interface e fluxo do menu (`main`)
- `exibir_menu()`
  Imprime as opções disponíveis no console para emissão de senhas, chamada por guichê, consulta de filas/painel e encerramento do programa.
- `main()`
  Coordena o laço principal de execução do sistema, lê as escolhas do usuário, valida entradas e despacha as operações para as funções auxiliares.

  
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

-- PAINEL DE CHAMADA --

Em atendimento: U001 (Urgente)
Proximas senhas: P001 P002 N001 
Em espera -> Urgente: 0 | Preferencial: 2 | Normal: 1


# Exemplo 3:

Opcao: 3

Ja existe um atendimento em andamento (U001 - Urgente).
Finalize o atendimento atual antes de chamar outro paciente.

Opcao: 4

Atendimento da senha U001 finalizado.

Opcao: 3

Chamando senha: P001 (Preferencial)

Opcao: 5

-- PAINEL DE CHAMADA --

Em atendimento: P001 (Preferencial)
Proximas senhas: P002 N001 
Em espera -> Urgente: 0 | Preferencial: 1 | Normal: 1


```bash
gcc -Wall -Wextra -o sistema_senhas main.c


