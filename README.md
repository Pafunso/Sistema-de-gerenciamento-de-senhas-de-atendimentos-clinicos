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

```bash
gcc -Wall -Wextra -o sistema_senhas main.c
