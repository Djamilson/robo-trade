//https://www.youtube.com/watch?v=KqK0OQovqz8&t=460s


Input
MediaCurta(8);
MediaLonga(80);
NumContrato(1);

Var
Media, MediaL, StopC, StopV : Float;

inicio

//PASSO 1 | RECEBER O VALOR  do indicador
MediaC := Media(MediaCuta,  Fechamento);
MediaL := Media(MediaLonga, Fechamento);

//PASSO 2 | SINAIS DE ENTRADA
se (IsBought) entao
  inicio 
    SellToCoverLimit(BuyPrice+((BuyPrice-StopC)*Alvo), NumContrato);
    SellToCoverStop(StopC, StopC-500, NumContrato);
  fim;

Se (IsSold) entao
  inicio
    //BuyToCoverLimit(SellPrice-(StopC*Alvo), NumContrato); alvo
    BuyToCoverLimit(SellPrice-(Abs(SellPrice-StopV)*Alvo), NumContrato);

    BuyToCoverStop(StopV, StopV+500, NumContrato); // stop
  Fim;


 Se (HasPosition = false) entao
    inicio
      Se (Minima > Minima[1]) e (Minima[1]<Minima[2]) e (Fechamento > Fechamento[1]) e (Fechamento < Maxima[2]) entao 
        inicio
          BuyStop(Maxima+5, Maxima+5+15, NumContrato)
          stopC := Minima-5;
        Fim;
      
      se (Maxima < Maxima[1]) e (Maxima[1] > Maxima[2]) e (Fechamento< Fechamento[1]) e (Fechamento > Minima[2]) entao
        inicio
          SellShortStop (Minima-5, Minima-5-15, NumContrato);
          StopV := Maxima+5;
        Fim;
      fim;
Fim;

//PASSO 3 | SINAIS  DE SAÍDA
//stopC  := (bF >= NivelC);
//stopV := (bF <= -NivelV);

//PASSO 4 | EXECUÇÃO DE ENTRADA
//se (not HasPosition) e SinalC entao BuyAtMarket
//senao se (not  HasPosition) e SinalV entao SellShortAtMarket;

//COMPRA
//VENDA
//PASSO 5 | EXECUÇÃO DE SAIDA
//PASSO 6 | EXECUÇÃO REVERSAO
//PASSO EXTRA 1 | COLORAÇÃO

//PLOTAGEM EXTR | PLOTAGEM





