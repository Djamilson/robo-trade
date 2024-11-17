//https://www.youtube.com/watch?v=KqK0OQovqz8&t=460s
Input
  MediaCurta(8),
  MediaLonga(80),
  Alvo(1.618), // Razão de alvo em relação ao risco
  RiscoPorOperacao(0.01), // Porcentagem do capital arriscada por operação
  CapitalInicial(100000); // Capital inicial para cálculo do risco

Var
  MediaC, MediaL, StopC, StopV, RiscoPorPonto : Float;
  NumContrato : Integer;

Inicio

// PASSO 1 | RECEBER OS VALORES DOS INDICADORES
MediaC := Media(MediaCurta, Fechamento);
MediaL := Media(MediaLonga, Fechamento);

// PASSO 2 | GERENCIAMENTO DE RISCO
// Calcular risco por ponto baseado na diferença entre preço de entrada e stop
RiscoPorPonto := Abs(StopC - Fechamento);

// Determinar número de contratos com base no risco permitido
Se RiscoPorPonto > 0 então
  NumContrato := Trunc((CapitalInicial * RiscoPorOperacao) / RiscoPorPonto)
Senão
  NumContrato := 1; // Valor mínimo caso haja algum erro

// PASSO 3 | SINAIS DE SAÍDA PARA POSIÇÕES EXISTENTES
Se (IsBought) então
  Inicio
    SellToCoverLimit(BuyPrice + ((BuyPrice - StopC) * Alvo), NumContrato); // Alvo
    SellToCoverStop(StopC, StopC - 500, NumContrato); // Stop
  Fim;

Se (IsSold) então
  Inicio
    BuyToCoverLimit(SellPrice - ((SellPrice - StopV) * Alvo), NumContrato); // Alvo
    BuyToCoverStop(StopV, StopV + 500, NumContrato); // Stop
  Fim;

// PASSO 4 | ENTRADA DE NOVAS OPERAÇÕES
Se (HasPosition = false) então
  Inicio
    // Sinal de compra
    Se (Minima > Minima[1]) e (Minima[1] < Minima[2]) e (Fechamento > Fechamento[1]) e (Fechamento < Maxima[2]) então
      Inicio
        BuyStop(Maxima + 5, Maxima + 5 + 15, NumContrato); // Entrada por rompimento
        StopC := Minima - 5; // Stop inicial
      Fim;

    // Sinal de venda
    Se (Maxima < Maxima[1]) e (Maxima[1] > Maxima[2]) e (Fechamento < Fechamento[1]) e (Fechamento > Minima[2]) então
      Inicio
        SellShortStop(Minima - 5, Minima - 5 - 15, NumContrato); // Entrada por rompimento
        StopV := Maxima + 5; // Stop inicial
      Fim;
  Fim;

// PASSO 5 | COLORAÇÃO E VISUALIZAÇÃO (OPCIONAL)
Se (IsBought) então PaintBar(clLime); // Verde para compra
Se (IsSold) então PaintBar(clRed); // Vermelho para venda

Fim;



