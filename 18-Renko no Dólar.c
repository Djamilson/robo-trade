//https://www.youtube.com/watch?v=s6DOsvv6xr8&t=901s


PARAMETRO
AmpAlvo(1.618);

Var
SinalC, SinalV: Boolean;
AlvoC, AlvoV, StopC, StopV, xVwap: Float;

begin

//PASSO 1 | RECEBER O VALOR  do indicador
//xVwap := Vwap(1);
//xVwap := Media(20,  Close);
//xVwap := Media(9,  Close);
xVwap := MediaExp(20,  Close);

//PASSO 2 | SINAIS DE ENTRADA
SinalC := ((Close[1] < Open[1]) e (Close[1] <= xVwap[1]) e (High[1] >= xVwap[1])) e ((Close > Open) e (Close > High[1]) e (Low < Low[1]));
SinalV := ((Close[1] > Open[1]) e (Close[1] >= xVwap[1]) e (Low[1] <= xVwap[1])) e ((Close < Open) e (Close < Low[1]) e (High > High[1]));

//PASSO 3 | SINAIS  DE SAÍDA
//stopC  := (bF >= NivelC);
//stopV := (bF <= -NivelV);

//PASSO 4 | EXECUÇÃO DE ENTRADA
//se (not HasPosition) e SinalC entao BuyAtMarket
//senao se (not  HasPosition) e SinalV entao SellShortAtMarket;
Se SinalV entao PaintBar(255);
Se SinalC entao PaintBar(clLime);

//COMPRA
Se (IsBought) entao
  inicio 
    SellToCoverLimit(BuyPrice+AlvoC*AmpAlvo);
    SellToCoverStop(StopC, StopC-500);
  fim;

//VENDA
Se (IsSold) entao
  inicio 
    BuyToCoverLimit(SellPrice-AlvoV*AmpAlvo);
    BuyToCoverStop(StopV, StopV+500);
  fim;

//PASSO 5 | EXECUÇÃO DE SAIDA
se (HasPosition = false) entao
  inicio
    AlvoC := (Close-(Low -MinPriceIncrement));
    AlvoV := Abs(Close - (High+MinPriceIncrement));
    //deve ser aqui dentro
    stopC:= (Low-MinPriceIncrement);
    stopV:= (High+MinPriceIncrement);

    se SinalV entao SellShortAtMarket;
    seSinalC entao BuyAtMarket;
  fim;
//PASSO 6 | EXECUÇÃO REVERSAO

//PASSO EXTRA 1 | COLORAÇÃO

//PLOTAGEM EXTR | PLOTAGEM


end;


