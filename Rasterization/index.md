# Rasterização de linhas utilizando Bresenham

## Introdução

O trabalho tem como objetivo rasterizar linhas através da implementação do algoritmo de Bresenham, utilizando o framework que simula acesso direto à memória de vídeo, este disponibilizado pelo Prof. Dr. Christian Azambuja Pagot. Na minha abordagem, para simplificar o código, defini uma classe para Pixel e outra para Color.

Para o entendimento do trabalho, é necessário saber que os monitores possuem uma matriz de pixels, onde a origem é localizada no canto superior esquerdo. Cada Pixel possui 4 componentes R(vermelho), G(verde), B(azul) e A(opacidade) onde se tem 1 byte dedicado para cada componente.

![Screen](https://rennanweslley.github.io/images/monitor1.png)

Para conseguir a posição do primeiro byte de um determinado pixel na matriz, usa-se a fórmula:
**`4*X + 4*Y*W`**, onde **W** é a largura da tela.

Três principais funções foram desenvolvidas:
- putPixel(): Que rasteriza um ponto na tela
- drawLine(): Que rasteriza uma linha de um ponto à outro
- drawTriangle(): Que raseteriza um triângulo através de três vértices


## Função putPixel()
A função putPixel() tem um proposta simples, recebe um pixel como parâmetro e o rasteriza na tela utilizando o FBptr que aponta para a coordenada (0,0). Utilizando da fórmula citada acima, dá para determinar a localização do pixel na matriz e em seguida basta setar os seus valores RGBA.

```C
    int indx = (4*pixel.getX()) + (4*pixel.getY()*IMAGE_WIDTH);
    
    FBptr[indx]   = pixel.getColor().getR();
    FBptr[++indx] = pixel.getColor().getG();
    FBptr[++indx] = pixel.getColor().getB();
    FBptr[++indx] = pixel.getColor().getA();
```


![putPixel](https://rennanweslley.github.io/images/putPixel.png)


## Função drawLine()
Função que rasteriza uma linha na tela através da implementação do algoritmo de Bresenham. Recebe dois pixels como parâmetros.

O algoritmo Bresenham não tem o intuito de formar uma linha perfeita, mas o mais próximo disto. De acordo com a distância do novo pixel à reta, ele decide se incrementa, ou não, para o lado.

![Bresenham Line](https://rennanweslley.github.io/images/Bresenham_line.png)

Porém o algoritmo só funciona para retas que estejam no 1º octante, e é preciso generalizar para todos os outros. Caso o coeficiente angular da reta seja maior que 1, inverte-se os valores de **X** e **Y**. O incremento do X e do Y varia de acordo com o octante em que a linha se encontra.

![Bresenham Diagram](https://rennanweslley.github.io/images/bresenhamGen.png)

Para isso, defini a função **sign()** para retornar o incremento do x e y.

```C
int sign(int a) {
    if(!a) 
        return 0;
    else if(a < 0) 
        return -1;
    else if(a > 0)
        return 1;
}

int signX = sign(end.getX() - start.getX()),
    signY = sign(end.getY() - start.getY());
```

#### Interpolação de Cores
Para a interpolação adotei uma abordagem simples mas não muito precisa. Consistem em comparar cada componente RGBA do pixel atual com sua respectiva no pixel final. Decrementa para quando a componente atual for menor do que a final, e incrementa no caso contrário. Assim garante que a cor do pixel atual se aproxime gradativamente da cor do pixel final.

```C
if(!c1.compare(c2)) {
    if(c1.getR() > c2.getR())
        c1.decR();
    else if(c1.getR() < c2.getR())
        c1.incR();

    if(c1.getG() > c2.getG())
        c1.decG();
    else if(c1.getG() < c2.getG())
        c1.incG();

    if(c1.getB() > c2.getB())
        c1.decB();
    else if(c1.getB() < c2.getB())
        c1.incB();

    if(c1.getA() > c2.getA())
        c1.decA();
    else if(c1.getA() < c2.getA())
        c1.incA();
}
```


![Interpolate](https://rennanweslley.github.io/images/Interpolate.png)


## Função drawTriangle()
A função recebe três pixels e desenha um triângulo. Basta chamar a função **drawLine()** três vezes.

```C
void drawTriangle(Pixel p1, Pixel p2, Pixel p3) {
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p1, p3);
}
```

![Triangle](https://rennanweslley.github.io/images/Triangle.png)


## Referências
- http://www.uobabylon.edu.iq/eprints/publication_2_22893_6215.pdf
- https://fr.wikipedia.org/wiki/Fichier:Bresenham_line.png
- ROGERS, D.F. Procedural Elements For Computer Graphics: 2. ed. Nova Iorque: McGraw-Hill, 1998
- Notas de Aula do Prof. Christian Pagot
