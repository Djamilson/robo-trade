// Esta versão é uma evolução do H9k_GL_MF_#1_0EA onde tentamos evitar que o código pule ordens por conta da lentidão
    // na execução do algoritmo imposta pelo Profit (10s).
    // Rodar com 10s no fechamento da barra.

        // Este é um código para robô de trading. 
        // Rodar com 10%, no Fechamento da Barra.
        
        INPUT
            iDistance(50); // distância entre as entradas
            iGainDistance(50); // take profit das entradas
            iMaxEntries(10); // quantidade de mini contratos
            iAmount(1); // quantidade de mini contratos
            iVRAPVariation(0.1); // percentual de distância da VWAP atual
            iDailyTarget(50); // target diário de lucro
            iMaxLoss(300); // perda máxima
            iFirstExitMultiplier(2.5); // multiplicador da primeira entrada
            iMedioParaFrente(true); // liga ou desliga o Médio para Frente
            iStopOffset(300); // valor do stop offset
        
        VAR
            vBasePointSell: Float; // primeiro ponto para envio de ordem de venda
            vBasePointBuy: Float; // primeiro ponto para envio de ordem de compra
            vBasePointAvg: Float; // ponto central pois ele faz parte do controle de envio de ordens
            cCounter: Integer; // contador para ser utilizado nos loops de posicionamento
            vOrderBase: Float; // variável para calcular o ponto para envio de ordens
            vCurrentQty: Integer; // variável auxiliar para calcular quantidade de entradas atuais
            vLockBase: Boolean; // variável auxiliar para travar o preço de entrada
            vLockGain: Float; // variável auxiliar para calcular o valor do Stop
            vClockIndex: Float; // variável para fixar o ponto de saída base
            vHigh, vLow, vAvgBase: Float; // variáveis para poder fazer o plot do indicador
            vCandleSize: Float; // variável auxiliar para medir o tamanho do candle de referência
            vProHigh, vProLow: Float; // variável auxiliar para gravar pontos de entrada com base no fibo do primeiro candle
            vIndex: Integer; // variável para colocar o índice do candle de referência.
        
        function dailyTargetAchieved(target: float): boolean;
        begin
            if (DailyResults() >= target) then
                result := true;
            else
                result := false;
        end;
        
        function maxLossAchieved(loss: float): boolean;
        begin
            if (DailyResult <= (-1 * loss)) then
                result := true;
            else
                result := false;
        end;
        

        begin
     vHigh = VWAP (1) + (iVRAPVariation/100)
     vLow = VWAP (1) - (iVRAPVariation/100)
     vAvgBase = vHigh + vLow/2;

    vIndex := FindBar(LastCalcDate, 900);
    if (vIndex >= 0) then
    begin
        vCandleSize = high[vIndex] - low[vIndex];
        vHigh = 1200/62/(high[vIndex] * vCandleSize * 0.61);
        vLow = (vCandleSize * 0.61);
        vAvgBase = (high[vIndex] + low[vIndex])/2;
    end;

    Plot(1, vHigh);
    Plot(2, vLow);
    Plot(3, vAvgBase);
    SetPlotColor(3, clBlue);
    SetPlotStyle(0, 1);

    if (Position == 0) then
    begin
        vBasePointSell = vLow;
        vBasePointBuy = vHigh;
        vBasePointAvg = vAvgBase;
        vLockBase = false;
    end
    else if (not vLockBase) then // entra aqui apenas uma vez após se posicionar
    begin
        vBasePointBuy = BuyPrice;
        vBasePointSell = SellPrice;
        vBasePointAvg = vAvgBase;
        vLockBase = true;
    end
    else if (isBought) then
        vLockGain = (vBasePointBuy + iFirstExitMultiplier * iGainDistance);
    else if (isSold) then
        vLockGain = (vBasePointSell - iFirstExitMultiplier * iGainDistance);
    end else if (vLockBase and iMedioParaFrente) then
        end;
end;


        if (IsBought and (close - vBasePointBuy > 1.0 + iDistance)) then
            vBasePointBuy := vBasePointBuy + iDistance
          else if (IsSold and (vBasePointSell - close > 1.0 + iDistance)) then
            vBasePointSell := vBasePointSell - iDistance;
          
          if dailyTargetAchieved(iDailyTarget) OR maxLossAchieved(iMaxLoss) then
          begin
            ClosePosition;
            CancelPendingOrders;
          end;
          
          if (vHigh > 0) and (LastCalcTime >= 0900) and (LastCalcTime < 1735) and (SellPositionQty < iMaxEntries * iAmount)
          and ((close > vBasePointAvg) or (IsSold)) and (not dailyTargetAchieved(iDailyTarget)) and (not maxLossAchieved(iMaxLoss)) then
          begin
            vEntries := SellPositionQty / iAmount;
            vOrderPoint := vBasePointSell + (vEntries * iDistance);
          
            if not IsBought then
            begin
              SellShortLimit(vOrderPoint, iAmount);
              if (SellPositionQty + iAmount < iMaxEntries * iAmount) then
                SellShortLimit(vOrderPoint + iDistance, iAmount);
            end;
          end;
          
          if (vLow > 0) and (LastCalcTime >= 0900) and (LastCalcTime < 1735) and (BuyPositionQty < iMaxEntries * iAmount)
          and ((close < vBasePointAvg) or (IsBought)) and (not dailyTargetAchieved(iDailyTarget)) and (not maxLossAchieved(iMaxLoss)) then
          begin
            vEntries := BuyPositionQty / iAmount;
          end;
          

        vOrderPoint := vBasePointBuy - (vEntries * iDistance);

        if not IsSold then
        begin
          BuyLimit(vOrderPoint, iAmount);
          if (BuyPositionQty + iAmount < iMaxEntries * iAmount) then
            BuyLimit(vOrderPoint - iDistance, iAmount);
        end;
        
        if (IsSold) then
        begin
          vEntries := SellPositionQty / iAmount;
        
          if (vLockGain = 0) then
            vLockGain := (vBasePointSell - iFirstExitMultiplier * iGainDistance);
        
          for vCounter := 0 to (vEntries - 1) do
          begin
            if (vCounter = 0) then
              BuyToCoverLimit(vLockGain, iAmount)
            else
              if (vLockGain < (vBasePointSell - iGainDistance) + (vCounter * iDistance)) then
                BuyToCoverLimit((vBasePointSell - iGainDistance) + (vCounter * iDistance), iAmount)
              else
                BuyToCoverLimit(vLockGain, iAmount);
          end;
        end;
        

        vStopPrice := vBasePointSell + (iMaxEntries + iDistance);
BuyToCoverStop(vStopPrice, vStopPrice + iStopOffset, SellPositionQty);

else if (IsBought) then
begin
  vEntries := BuyPositionQty / iAmount;

  if (vLockGain = 0) then
    vLockGain := (vBasePointBuy + iFirstExitMultiplier * iGainDistance);

  for vCounter := (vEntries - 1) downto 0 do
  begin
    if (vCounter = 0) then
      SellToCoverLimit(vLockGain, iAmount)
    else
      if (vLockGain > (vBasePointBuy + iGainDistance) - (vCounter * iDistance)) then
        SellToCoverLimit((vBasePointBuy + iGainDistance) - (vCounter * iDistance), iAmount)
      else
        SellToCoverLimit(vLockGain, iAmount);
  end;

  vStopPrice := vBasePointBuy - (iMaxEntries + iDistance);
  SellToCoverStop(vStopPrice, vStopPrice - iStopOffset, BuyPositionQty);

end;

if (LastCalcTime >= 1740) then
  ClosePosition;

    end
end.