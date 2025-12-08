import argparse
import random
import sys

def gerar_sequencia_requisicoes(num_requisicoes, max_pagina):
    """Gera uma lista de índices de página aleatórios."""
    # Os índices de página variam de 0 a P-1, onde P é o número máximo de páginas (max_pagina).
    # O random.randint(a, b) inclui ambos os limites.
    sequencia = [random.randint(0, max_pagina - 1) for _ in range(num_requisicoes)]
    # Retorna a sequência como uma string de números separados por espaço
    return " ".join(map(str, sequencia))

def gerar_entrada_simulador(mfisica, mvirtual, arquitetura, num_paginas, num_sequencias,
                           max_req_por_seq, min_req_por_seq):
    """Gera o conteúdo completo da entrada conforme as especificações."""
    
    # 1. Parâmetros de Configuração
    output = f"{mfisica}\n"
    output += f"{mvirtual}\n"
    output += f"{arquitetura}\n"
    output += f"{num_paginas}\n"
    
    # Linha em branco
    output += "\n"
    
    # 2. Número de Sequências
    output += f"{num_sequencias}\n"

    output += "\n"
    
    # 3. Sequências de Requisições
    for i in range(num_sequencias):
        # Gera um número aleatório de requisições para esta sequência
        num_requisicoes = random.randint(min_req_por_seq, max_req_por_seq)
        
        # Gera a sequência de páginas requisitadas
        sequencia = gerar_sequencia_requisicoes(num_requisicoes, num_paginas)
        
        # Linha em branco separando as sequências (se não for a primeira)
        if i > 0:
            output += "\n"
            
        # Adiciona o número de requisições e a sequência
        output += f"{num_requisicoes}\n"
        output += f"{sequencia}\n"
        
    return output

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description="Script para gerar arquivos de entrada para o simulador de Memória Virtual.",
        formatter_class=argparse.RawTextHelpFormatter
    )

    # Parâmetros obrigatórios e opcionais com valores padrão
    parser.add_argument(
        '-p', '--num_paginas', type=int, default=16,
        help="Número total de páginas virtuais (P). Os índices variam de 0 a P-1. (Padrão: 16)"
    )
    parser.add_argument(
        '-s', '--num_sequencias', type=int, default=3,
        help="Quantidade de sequências de requisições a serem geradas. (Padrão: 3)"
    )
    parser.add_argument(
        '--max_req', type=int, default=100,
        help="Número MÁXIMO de requisições por sequência. (Padrão: 100)"
    )
    parser.add_argument(
        '--min_req', type=int, default=10,
        help="Número MÍNIMO de requisições por sequência. (Padrão: 10)"
    )
    
    # Parâmetros de configuração fixos (podem ser ajustados conforme o seu caso)
    parser.add_argument(
        '--mfisica', type=int, default=4096,
        help="Tamanho da Memória Física (M) em bytes. (Padrão: 4096)"
    )
    parser.add_argument(
        '--mvirtual', type=int, default=16384,
        help="Tamanho da Memória Virtual (V) em bytes. (Padrão: 16384)"
    )
    parser.add_argument(
        '--arquitetura', type=str, default='x86', choices=['x86', 'x64', '286', '264'],
        help="Arquitetura de endereçamento. (Padrão: x86)"
    )

    args = parser.parse_args()
    
    # Verifica a consistência dos argumentos
    if args.min_req > args.max_req:
        print("Erro: O número mínimo de requisições (--min_req) não pode ser maior que o máximo (--max_req).", file=sys.stderr)
        sys.exit(1)
        
    if args.num_paginas <= 0:
        print("Erro: O número de páginas (--num_paginas) deve ser positivo.", file=sys.stderr)
        sys.exit(1)

    # Gera o conteúdo do arquivo
    conteudo_entrada = gerar_entrada_simulador(
        mfisica=args.mfisica,
        mvirtual=args.mvirtual,
        arquitetura=args.arquitetura,
        num_paginas=args.num_paginas,
        num_sequencias=args.num_sequencias,
        max_req_por_seq=args.max_req,
        min_req_por_seq=args.min_req
    )

    # Imprime o resultado na saída padrão (stdout)
    print(conteudo_entrada, end='')