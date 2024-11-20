-- Parâmetros fixos (constantes)
local preçoBase = close;
local tolerancia = 0.1; -- Tolerância para identificar a mesma região de preço
local minToques = 2;    -- Mínimo de toques na região
local rompimentoPerc = 0.5; -- Percentual de rompimento acima/abaixo
local riscoPorTrade = 1; -- Risco por trade (% do capital)
local maxPerdaDiaria = 5; -- Perda máxima diária (% do capital)
local maxLucroDiario = 5; -- Meta de lucro diário (% do capital)
local stopLoss = 0.5;     -- Stop Loss em %
local takeProfit = 1;     -- Take Profit em %
local capitalInicial = 1000; -- Capital inicial em dinheiro

-- Posição máxima
local maxPosicoes = 2;
local posicoesAbertas = 0;

-- Variáveis globais
var regiãoDePreço = 0;
var contagemToques = 0;
var lucroDiario = 0;
var perdaDiaria = 0;
var capitalAtual = capitalInicial;

-- Função para atualizar o trailing stop
function atualizaTrailingStop(precoAtual, precoEntrada, trailingStopPercentual, posicao)
    -- Calcula o novo preço do stop com base no preço atual
    local novoTrailingStop;
    if posicao == "compra" then
        -- Para compras, o stop segue o preço subindo
        novoTrailingStop = precoAtual - (trailingStopPercentual / 100) * precoAtual;
        if novoTrailingStop > precoEntrada then
            setStopLoss(novoTrailingStop, abs);
        end
    elseif posicao == "venda" then
        -- Para vendas, o stop segue o preço descendo
        novoTrailingStop = precoAtual + (trailingStopPercentual / 100) * precoAtual;
        if novoTrailingStop < precoEntrada then
            setStopLoss(novoTrailingStop, abs);
        end
    end
end

-- Calcula o tamanho da posição com base no risco permitido
function calculaTamanhoPosicao(precoAtual, riscoPorcentagem, stopPercentual)
    local riscoEmDinheiro = (riscoPorcentagem / 100) * capitalAtual;
    local stopValor = (stopPercentual / 100) * precoAtual;
    if stopValor > 0 then
        return math.floor(riscoEmDinheiro / stopValor);
    else
        return 0;
    end
end

-- Atualiza os lucros e perdas diários
function atualizaRisco(resultadoTrade)
    if resultadoTrade > 0 then
        lucroDiario = lucroDiario + resultadoTrade;
    else
        perdaDiaria = perdaDiaria + math.abs(resultadoTrade);
    end
    capitalAtual = capitalAtual + resultadoTrade;
end

-- Verifica se o limite de risco diário foi atingido
function verificaLimites()
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
if math.abs(preçoBase - regiãoDePreço) <= tolerancia then
    contagemToques = contagemToques + 1;
else
    contagemToques = 0;
    regiãoDePreço = preçoBase; -- Atualiza a região caso não esteja na mesma
end

-- Verifica se houve rompimento e executa a operação
if contagemToques >= minToques and verificaLimites() and posicoesAbertas < maxPosicoes then
    local tamanhoPosicao = calculaTamanhoPosicao(preçoBase, riscoPorTrade, stopLoss);
    if preçoBase > regiãoDePreço * (1 + rompimentoPerc / 100) then
        -- Rompimento para cima (compra)
        buy size tamanhoPosicao at market;
        setStopLoss(stopLoss, pct);
        setTakeProfit(takeProfit, pct);
        atualizaRisco(tamanhoPosicao * takeProfit / 100 * preçoBase);
        posicoesAbertas = posicoesAbertas + 1;
    elseif preçoBase < regiãoDePreço * (1 - rompimentoPerc / 100) then
        -- Rompimento para baixo (venda)
        sellShort size tamanhoPosicao at market;
        setStopLoss(stopLoss, pct);
        setTakeProfit(takeProfit, pct);
        atualizaRisco(tamanhoPosicao * takeProfit / 100 * preçoBase);
        posicoesAbertas = posicoesAbertas + 1;
    end
end

-- Atualizar o trailing stop
if posicoesAbertas > 0 then
    for i = 1, posicoesAbertas do
        atualizaTrailingStop(close, precoEntrada, 0.5, "compra"); -- Exemplo para uma compra
    end
end

-- Função para fechar uma posição
function fechaPosicao(posicao)
    -- Lógica para fechar a posição
    posicoesAbertas = posicoesAbertas - 1;
end
