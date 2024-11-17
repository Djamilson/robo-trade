//https://www.youtube.com/watch?v=s6DOsvv6xr8&t=901s
// Configuração das variáveis
inputs:
    media_curta(9), 
    media_longa(21), 
    risco_por_operacao(0.01), 
    alvo_risco(1.5);

vars:
    MediaC(0), 
    MediaL(0), 
    ATR(0), 
    stop_c(0), 
    stop_v(0), 
    preco_entrada(0), 
    posicao(0), 
    perda_dia(0),
    dia_atual(0);

// Cálculo de indicadores
MediaC = Average(Close, media_curta);
MediaL = Average(Close, media_longa);
ATR = AvgTrueRange(14);

// Resetar perdas acumuladas no início do dia
if Date > dia_atual then begin
    perda_dia = 0;
    dia_atual = Date;
end;

// Gerenciamento de risco: parar operações se perda diária >= 1% do capital
if perda_dia >= 0.01 * AccountSize then
    return;

// Estratégia de entrada
if posicao = 0 then begin
    if MediaC > MediaL and Low > Low[1] and Close > Close[1] then begin
        preco_entrada = High + 5;
        stop_c = Low - ATR;
        posicao = 1; // Compra
        Buy ("EntradaCompra") next bar at preco_entrada stop;
    end
    else if MediaC < MediaL and High < High[1] and Close < Close[1] then begin
        preco_entrada = Low - 5;
        stop_v = High + ATR;
        posicao = -1; // Venda
        SellShort ("EntradaVenda") next bar at preco_entrada stop;
    end;
end;

// Gerenciar saídas (compra)
if posicao = 1 then begin
    // Alvo e Stop
    Sell ("SaidaLucro") next bar at (preco_entrada + (preco_entrada - stop_c) * alvo_risco) limit;
    Sell ("SaidaStop") next bar at stop_c stop;
end;

// Gerenciar saídas (venda)
if posicao = -1 then begin
    // Alvo e Stop
    BuyToCover ("SaidaLucro") next bar at (preco_entrada - (stop_v - preco_entrada) * alvo_risco) limit;
    BuyToCover ("SaidaStop") next bar at stop_v stop;
end;

// Atualizar perdas diárias
if MarketPosition = 0 then begin
    if LastTradeProfit < 0 then
        perda_dia = perda_dia + absvalue(LastTradeProfit);
end;
