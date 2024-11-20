-- Parâmetros fixos (constantes)
const preçoBase = close;
const tolerancia = 0.1; -- Tolerância para identificar a mesma região de preço
const minToques = 2;    -- Mínimo de toques na região
const rompimentoPerc = 0.5; -- Percentual de rompimento acima/abaixo
const riscoPorTrade = 1; -- Risco por trade (% do capital)
const maxPerdaDiaria = 5; -- Perda máxima diária (% do capital)
const maxLucroDiario = 5; -- Meta de lucro diário (% do capital)
const stopLoss = 0.5;     -- Stop Loss em %
const takeProfit = 1;     -- Take Profit em %
const capitalInicial = 10000; -- Capital inicial em dinheiro

-- Variáveis globais
var regiãoDePreço = 0;
var contagemToques = 0;
var lucroDiario = 0;
var perdaDiaria = 0;
var capitalAtual = capitalInicial;

-- Calcula o tamanho da posição com base no risco permitido
função calculaTamanhoPosicao(precoAtual, riscoPorcentagem, stopPercentual)
    local riscoEmDinheiro = (riscoPorcentagem / 100) * capitalAtual;
    local stopValor = (stopPercentual / 100) * precoAtual;
    if stopValor > 0 then
        return math.floor(riscoEmDinheiro / stopValor);
    else
        return 0;
    end
end

-- Atualiza os lucros e perdas diários
função atualizaRisco(resultadoTrade)
    if resultadoTrade > 0 then
        lucroDiario = lucroDiario + resultadoTrade;
    else
        perdaDiaria = perdaDiaria + math.abs(resultadoTrade);
    end
    capitalAtual = capitalAtual + resultadoTrade;
end

-- Verifica se o limite de risco diário foi atingido
função verificaLimites()
    if perdaDiaria >= (maxPerdaDiaria / 100) * capitalInicial then
        print("Limite de perda diária atingido. Operações encerradas.");
        return false;
    elseif lucroDiario >= (maxLucroDiario / 100) * capitalInicial then
        print("Meta de lucro diário atingida. Operações encerradas.");
        return false;
    end
    return true;
end

-- Identifica a região de preço
if abs(preçoBase - regiãoDePreço) <= tolerancia then
    contagemToques = contagemToques + 1;
else
    contagemToques = 0;
    regiãoDePreço = preçoBase; -- Atualiza a região caso não esteja na mesma
end

-- Verifica se houve rompimento e executa a operação
if contagemToques >= minToques and verificaLimites() then
    local tamanhoPosicao = calculaTamanhoPosicao(preçoBase, riscoPorTrade, stopLoss);
    if preçoBase > regiãoDePreço * (1 + rompimentoPerc / 100) then
        -- Rompimento para cima (compra)
        buy size tamanhoPosicao at market;
        setStopLoss(stopLoss, pct);
        setTakeProfit(takeProfit, pct);
        atualizaRisco(tamanhoPosicao * takeProfit / 100 * preçoBase);
    elseif preçoBase < regiãoDePreço * (1 - rompimentoPerc / 100) then
        -- Rompimento para baixo (venda)
        sellShort size tamanhoPosicao at market;
        setStopLoss(stopLoss, pct);
        setTakeProfit(takeProfit, pct);
        atualizaRisco(tamanhoPosicao * takeProfit / 100 * preçoBase);
    end
end
