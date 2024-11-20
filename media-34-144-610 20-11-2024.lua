const 
MediaCurta(34);
MediaLonga(144);
MediaSuperLonga(610);

Var

vMediaCurta, vMediaLonga, vMediaSuperLonga : Float;

Beging

vMediaCurta := MediaExp(MediaCurta, close);
vMediaLonga := MediaExp(MediaLonga, close);
vMediaSuperLonga := MediaExp(MediaSuperLonga, close);


Plot(vMerdiaCurta);
Plot2(vMediaLonga);
Plot3(vMediaSuperLoga);

End;