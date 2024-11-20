-- Variáveis globais
var operacoesAbertas = {}; -- Lista de operações abertas
var lucroTotal = 0;

-- Função para listar operações abertas
função listarOperacoes()
    print("===== OPERAÇÕES ABERTAS =====");
    for i = 1, #operacoesAbertas do
        local operacao = operacoesAbertas[i];
        local lucroAtual = (close - operacao.precoEntrada) * operacao.tamanho * (operacao.tipo == "compra" and 1 or -1);
        print("ID: ", operacao.id, 
              " | Tipo: ", operacao.tipo, 
              " | Preço Entrada: ", operacao.precoEntrada, 
              " | Tamanho: ", operacao.tamanho, 
              " | Lucro/Prejuízo: ", lucroAtual);
    end
    print("=============================");
end

-- Função para encerrar uma operação
função encerrarOperacao(id)
    for i = 1, #operacoesAbertas do
        if operacoesAbertas[i].id == id then
            local operacao = operacoesAbertas[i];
            local lucroFinal = (close - operacao.precoEntrada) * operacao.tamanho * (operacao.tipo == "compra" and 1 or -1);
            lucroTotal = lucroTotal + lucroFinal;
            print("Operação encerrada! ID: ", operacao.id, 
                  " | Tipo: ", operacao.tipo, 
                  " | Lucro/Prejuízo Final: ", lucroFinal);
            table.remove(operacoesAbertas, i); -- Remove a operação da lista
            return;
        end
    end
    print("Operação não encontrada! ID: ", id);
end

-- Simulação de abertura de operação (apenas para testes)
função abrirOperacao(tipo, precoEntrada, tamanho)
    local id = #operacoesAbertas + 1;
    table.insert(operacoesAbertas, {
        id = id,
        tipo = tipo,
        precoEntrada = precoEntrada,
        tamanho = tamanho
    });
    print("Operação aberta! ID: ", id, " | Tipo: ", tipo, " | Preço Entrada: ", precoEntrada, " | Tamanho: ", tamanho);
end

-- Testes (Apenas para demonstração)
if isBacktest then
    -- Abre operações de teste
    abrirOperacao("compra", 100, 1); -- Compra de 1 contrato a 100
    abrirOperacao("venda", 105, 2);  -- Venda de 2 contratos a 105
    
    -- Lista operações abertas
    listarOperacoes();

    -- Fecha uma operação
    encerrarOperacao(1); -- Fecha a operação com ID 1

    -- Lista operações após fechamento
    listarOperacoes();

    -- Exibe lucro total acumulado
    print("Lucro/Prejuízo Total Acumulado: ", lucroTotal);
end
