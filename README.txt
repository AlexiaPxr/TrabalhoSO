Trabalho Prático – Sistemas Operacionais
Simulador de Memória Virtual com Paginação por Demanda
Autores: Aléxia Pereira Quaresma, Deignaura Gonçalves Ribeiro, Pedro Marçal Ballesteros

--------------------------------------------------------------------
1. Descrição do Projeto
--------------------------------------------------------------------

Este trabalho implementa um simulador de Memória Virtual utilizando
Paginação por Demanda. O programa processa sequências de requisições
de páginas e avalia quatro políticas de substituição:

• FIFO (First-In, First-Out)
• RAND (Aleatória)
• LRU (Least Recently Used)
• MIN / Ótima (OPT)

Para cada sequência, o simulador informa:

• Tempo total de execução (em segundos, arredondado)
• Número de Page Faults
• Páginas presentes no Swap ao final

O objetivo é comparar o comportamento das políticas e compreender
o funcionamento interno de um sistema de memória paginada.

--------------------------------------------------------------------
2. Compilação
--------------------------------------------------------------------

Linux / WSL:
    g++ -std=c++17 -O2 src/main.cpp -o bin/exec

Windows (PowerShell + MinGW):
    g++ -std=c++17 -O2 src/main.cpp -o bin/exec.exe


--------------------------------------------------------------------
3. Execução
--------------------------------------------------------------------

IMPORTANTE: o operador "<" não funciona no PowerShell.

Linux / WSL:
    ./bin/exec < entrada.txt > saida.txt

Windows (PowerShell):
    Get-Content entrada.txt | .\bin\exec.exe > saida.txt

Windows (CMD clássico):
    bin\exec.exe < entrada.txt > saida.txt


--------------------------------------------------------------------
4. Formato da Entrada
--------------------------------------------------------------------

A entrada deve conter:

Linha 1: Tamanho da Memória Física (M) em bytes  
Linha 2: Tamanho da Memória Virtual (V) em bytes  
Linha 3: Arquitetura (x86 ou x64)  
Linha 4: Número de páginas virtuais (P)  
Linha 5: (linha em branco)

Linha 6: Número de sequências (N)

Para cada sequência:
    Linha A: Número de requisições (R)
    Linha B: R valores representando as páginas solicitadas


--------------------------------------------------------------------
5. Formato da Saída
--------------------------------------------------------------------

Para cada execução, o programa imprime:

1) Parâmetros deriváveis do sistema:
   - Tamanho da Página (SP)
   - Número de Frames da Memória Física
   - Tamanho mínimo do Swap (em bytes)

2) Para cada sequência:
   - Quantidade R
   - A sequência de páginas
   - Para cada política (FIFO, RAND, LRU, MIN):
        • Tempo total (segundos)
        • Número de Page Faults
        • Páginas no Swap ao final


--------------------------------------------------------------------
6. Estrutura do Repositório
--------------------------------------------------------------------

/src/main.cpp        -> Código-fonte principal  
/bin/exec.exe        -> Executável (Windows)  
/entrada.txt         -> Exemplo de entrada  
/saida.txt           -> Exemplo de saída  
/testcases/          -> Casos de teste (small, medium, large)  
/gen/                -> Gerador de casos fornecido  
/gen/gen.py          -> Script gerador  
/README.txt          -> Este documento  
/.gitignore          -> Arquivo de ignorados do Git


--------------------------------------------------------------------
7. Observações de Implementação
--------------------------------------------------------------------

• Código modularizado e organizado conforme pedido (sem AI bloat).  
• RAND utiliza semente fixa (42) para resultados reproduzíveis.  
• Políticas implementadas conforme definição teórica oficial.  
• Memória reinicializada antes de cada simulação.  
• Compatível com Linux, Windows, WSL e CMD.  


--------------------------------------------------------------------
8. Autores
--------------------------------------------------------------------

Aléxia Pereira Quaresma  
Deignaura Gonçalves Ribeiro  
Pedro Marçal Ballesteros  

--------------------------------------------------------------------
FIM
