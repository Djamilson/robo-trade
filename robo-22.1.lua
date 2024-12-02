// Configurações e parâmetros principais
INPUT
    iDistance(10); // Distância entre as ordens
    iGainDistance(10); // Distância para Take Profit
    iMaxEntries(10); // Máximo de entradas permitidas
    iAmount(1); // Quantidade de contratos por entrada
    iVRAPVariation(0.1); // Percentual de distância da VWAP atual
    iDailyTarget(50); // Lucro diário desejado
    iMaxLoss(50); // Perda diária máxima (5% de R$1000)
    iFirstExitMultiplier(2.5); // Multiplicador para a primeira saída
    iMedioParaFrente(true); // Estratégia de Médio para Frente ativada
    iStopOffset(300); // Valor de offset do stop loss

VAR
    vBasePointSell: Float;
    vBasePointBuy: Float;
    vBasePointAvg: Float;
    vLockBase: Boolean;
    vLockGain: Float;
    vHigh, vLow, vAvgBase: Float;
    vIndex: Integer;
    vStopPrice: Float;

// Funções auxiliares
function dailyTargetAchieved(target: float): boolean;
begin
    if (DailyResults() >= target) then
        result := true
    else
        result := false;
end;

function maxLossAchieved(loss: float): boolean;
begin
    if (DailyResults() <= (-1 * loss)) then
        result := true
    else
        result := false;
end;

// Início do programa principal
begin
    vHigh := VWAP(1) + (iVRAPVariation / 100);
    vLow := VWAP(1) - (iVRAPVariation / 100);
    vAvgBase := (vHigh + vLow) / 2;

    // Plotagem dos níveis no gráfico
    Plot(1, vHigh); // Plota o nível superior
    SetPlotColor(1, clGreen); // Cor verde para o nível superior
    Plot(2, vLow); // Plota o nível inferior
    SetPlotColor(2, clRed); // Cor vermelha para o nível inferior
    Plot(3, vAvgBase); // Plota o nível médio
    SetPlotColor(3, clBlue); // Cor azul para o nível médio
    SetPlotStyle(3, 2); // Estilo pontilhado para a média

    // Ajuste inicial das variáveis de posição
    if (Position = 0) then
    begin
        vBasePointSell := vLow;
        vBasePointBuy := vHigh;
        vBasePointAvg := vAvgBase;
        vLockBase := false;
    end
    else if (not vLockBase) then
    begin
        vBasePointBuy := BuyPrice;
        vBasePointSell := SellPrice;
        vBasePointAvg := vAvgBase;
        vLockBase := true;
    end;

    // Controle de encerramento com base no lucro ou perda diária
    if dailyTargetAchieved(iDailyTarget) or maxLossAchieved(iMaxLoss) then
    begin
        ClosePosition;
        CancelPendingOrders;
        Exit;
    end;

    // Estratégia de entrada e saída
    if (LastCalcTime >= 0900) and (LastCalcTime < 1640) then
    begin
        // Entrada de venda
        if (Position <= 0) and (Close < vBasePointAvg) then
        begin
            SellShortLimit(vBasePointSell, iAmount);
            vBasePointSell := vBasePointSell - iDistance;
        end;

        // Entrada de compra
        if (Position >= 0) and (Close > vBasePointAvg) then
        begin
            BuyLimit(vBasePointBuy, iAmount);
            vBasePointBuy := vBasePointBuy + iDistance;
        end;
    end;

    // Saída com lucro
    if (IsBought) then
        SellToCoverLimit(vBasePointBuy + iGainDistance, iAmount);
    if (IsSold) then
        BuyToCoverLimit(vBasePointSell - iGainDistance, iAmount);

    // Saída com Stop
    if (IsBought) then
    begin
        vStopPrice := vBasePointBuy - iStopOffset;
        SellToCoverStop(vStopPrice, vStopPrice - iStopOffset, BuyPositionQty);
    end
    else if (IsSold) then
    begin
        vStopPrice := vBasePointSell + iStopOffset;
        BuyToCoverStop(vStopPrice, vStopPrice + iStopOffset, SellPositionQty);
    end;

    // Encerramento forçado às 16h40
    if (LastCalcTime >= 1640) then
    begin
        ClosePosition;
        CancelPendingOrders;
    end;
end;
