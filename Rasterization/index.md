# Rasterização de linhas utilizando Bresenham

## Introdução

O trabalho tem como objetivo rasterizar linhas através da implementação do algoritmo de Bresenham, utilizando o framework que simula acesso direto à memória de vídeo, este disponibilizado pelo Prof. Dr. Christian Azambuja Pagot.

Para o entendimento do trabalho, é necessário saber que os monitores possuem uma matriz de pixels, onde a origem é localizada no canto superior esquerdo. Cada Pixel possui 4 componentes R(vermelho), G(verde), B(azul) e A(opacidade) onde se tem 1 byte dedicado para cada componente.

![Screen](https://rennanweslley.github.io/images/monitor1.png)

Para conseguir a posição do primeiro byte de um determinado pixel na matriz, usa-se a fórmula:
**`4*X + 4*Y*W`**, onde **W** é a largura da tela.

Três principais funções foram desenvolvidas:
- putPixel(): Que rasteriza um ponto na tela
- drawLine(): Que rasteriza uma linha de um ponto à outro
- drawTriangle(): Que raseteriza um triângulo através de três vértices


## Função putPixel()
A função putPixel() tem um proposta simples, recebe um pixel como parâmetro e o rasteriza na tela utilizando o fb_ptr que aponta para a coordenada (0,0) do nosso "framebuffer". Utilizando da fórmula citada acima, dá para determinar a localização do pixel na matriz e em seguida basta setar os seus valores RGBA.

```C
void putPixel(pixel_t pixel) {
    int pos = pixel.x * 4 + pixel.y * IMAGE_WIDTH * 4;
    
    fb_ptr[pos++] = pixel.color.r;
    fb_ptr[pos++] = pixel.color.g;
    fb_ptr[pos++] = pixel.color.b;
    fb_ptr[pos]   = pixel.color.a;
}
```

![putPixel](https://rennanweslley.github.io/images/putPixel.png)


## Função drawLine()
Função que rasteriza uma linha na tela através da implementação de uma variação do algoritmo de Bresenham, o algoritmo do ponto médio. Recebe dois pixels como parâmetros.

O algoritmo Bresenham não tem o intuito de formar uma linha perfeita, mas o mais próximo disto. De acordo com a distância do novo pixel à reta, ele decide se o próximo pixel a ser pintado será o mais ao Leste (x++, y) ou o mais ao nordeste (x++,y++).

![Bresenham Line](https://rennanweslley.github.io/images/Bresenham_line.png)

Porém o algoritmo só funciona para retas que estejam no 1º octante, e é preciso generalizar para todos os outros. Caso o coeficiente angular da reta seja maior que 1, inverte-se os valores de **X** e **Y**. O incremento do X e do Y varia de acordo com o octante em que a linha se encontra.

![Bresenham Diagram](https://rennanweslley.github.io/images/bresenhamGen.png)

Para isso, foi definida a função **inc()** para retornar o incremento do x e do y.

```C
int inc(int a) {
    int x;
    
    if(!a) 
        x = 0;
    else if(a < 0) 
        x = -1;
    else if(a > 0)
        x = 1;
    
    return x;
}
    
int incX = inc(b.x - a.x),
    incY = inc(b.y - a.y);
```

#### Interpolação de Cores
Para a interpolação adotei uma abordagem simples mas não muito precisa. Consistem em comparar cada componente RGBA do pixel atual com sua respectiva no pixel final. Decrementa para quando a componente atual for menor do que a final, e incrementa no caso contrário. Assim garante que a cor do pixel atual se aproxime gradativamente da cor do pixel final.

```C
color_t interpolate(pixel_t iP, pixel_t mP, pixel_t fP) {
    double p = dist(mP, fP)/dist(iP, fP);
    
    color_t newColor = {p*iP.color.r + (1-p)*fP.color.r,
                        p*iP.color.g + (1-p)*fP.color.g,
                        p*iP.color.b + (1-p)*fP.color.b,
                        p*iP.color.a + (1-p)*fP.color.a};
    
    return newColor;
}
```

![Interpolate](https://rennanweslley.github.io/images/Interpolate.png)


## Função drawTriangle()
A função recebe três pixels e desenha um triângulo. Basta chamar a função **drawLine()** três vezes.

```C
void drawTriangle(pixel_t p1, pixel_t p2, pixel_t p3) {
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, p1);
}
```

![Triangle](https://rennanweslley.github.io/images/Triangle.png)


## Referências
- http://www.uobabylon.edu.iq/eprints/publication_2_22893_6215.pdf
- https://fr.wikipedia.org/wiki/Fichier:Bresenham_line.png
- ROGERS, D.F. Procedural Elements For Computer Graphics: 2. ed. Nova Iorque: McGraw-Hill, 1998
- Notas de Aula do Prof. Christian Pagot
