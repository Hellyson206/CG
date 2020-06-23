# Rasterização de linhas utilizando Bresenham

## Introdução

O trabalho tem como objetivo rasterizar linhas através da implementação do algoritmo de Bresenham, utilizando o framework que simula acesso direto à memória de vídeo, disponibilizado pelo Prof. Dr. Christian Azambuja Pagot.

Para o entendimento do trabalho, é necessário saber que os monitores possuem uma matriz de pixels, onde a origem é localizada no canto superior esquerdo. Cada Pixel possui 4 componentes R(vermelho), G(verde), B(azul) e A(opacidade) onde se tem 1 byte dedicado para cada componente.

![Screen](https://github.com/Shanksir/CG/blob/master/Rasterization/images/monitor1.png)

Para conseguir a posição do primeiro byte de um determinado pixel na matriz, usa-se a fórmula:
**`4*X + 4*Y*W`**, onde **W** é a largura da tela.

Três principais funções foram implementadas:
- putPixel(): Que rasteriza um ponto na tela
- drawLine(): Que rasteriza uma linha de um ponto à outro
- drawTriangle(): Que raseteriza um triângulo através de três vértices


## Função putPixel()
A função putPixel() tem um proposta simples, recebe um pixel como parâmetro e o rasteriza na tela utilizando o fb_ptr que aponta para a coordenada (0,0) do nosso "framebuffer". Utilizando-se da fórmula citada acima, dá para determinar a localização do pixel na tela e em seguida basta setar os seus valores RGBA.

```C
void putPixel(pixel_t pixel) {
    int pos = pixel.x * 4 + pixel.y * IMAGE_WIDTH * 4;
    
    fb_ptr[pos++] = pixel.color.r;
    fb_ptr[pos++] = pixel.color.g;
    fb_ptr[pos++] = pixel.color.b;
    fb_ptr[pos]   = pixel.color.a;
}
```

![putPixel](https://github.com/Shanksir/CG/blob/master/Rasterization/images/putPixel.png)


## Função drawLine()
Função que rasteriza uma linha na tela através da implementação de uma variação do algoritmo de Bresenham, o algoritmo do ponto médio. Recebe dois pixels como parâmetros.

O algoritmo Bresenham não tem o intuito de formar uma linha perfeita, mas o mais próximo disto. De acordo com a distância do novo pixel à reta, ele decide se o próximo pixel a ser pintado será o mais ao Leste (x++, y) ou o mais ao nordeste (x++,y++).

![Bresenham Line](https://github.com/Shanksir/CG/blob/master/Rasterization/images/Bresenham_line.png)

Porém o algoritmo só funciona para retas que estejam no 1º octante, e é preciso generalizar para todos os outros. Caso o coeficiente angular da reta seja maior que 1, inverte-se os valores de **X** e **Y**. O incremento do X e do Y varia de acordo com o octante em que a linha se encontra.

![Bresenham Diagram](https://github.com/Shanksir/CG/blob/master/Rasterization/images/bresenhamGen.png)

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
Para a interpolação adotamos uma abordagem simples. Consiste na comparação de duas distâncias (parcial e total). Essa comparação se dá através de uma divisão simples que gera um fator de porcentagem que indica o quão próximo o pixel atual está do pixel final. A medida em que o pixel atual for se aproximando do final, o valor do fator de porcentagem também será diferente, o que influenciará diretamente na nova cor a ser gerada, fazendo com que a mesma se aproxime gradativamente da cor do pixel final.

Para o cálculo da distância foi definida a função **dist()**:

```C
double dist(pixel_t a, pixel_t b) {
    return sqrt(pow(b.x-a.x, 2) + pow(b.y-a.y, 2));
}
```

Onde **sqrt()** e **pow(value, exp)** são funções da biblioteca **math.h**. A função sqrt() calcula a raiz quadrada e a função pow() calcula um valor elevado à um expoente que é passado por parâmetro.

A interpolação das cores acontece na função **interpolate()**:

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
Onde iP é o pixel incial da reta, mP é pixel atual a ser rasterizado e fP é o pixel final da reta. O fator de porcentagem **p** será gerado pela distância parcial (distância entre mP e fP) dividida pela distância total (distância entre iP e fP). Para gerar a cor do pixel atual, é realizado o cálculo: **`p*iP.color + (1-p)*fP.color`** para todas as componentes RGBA.

![Interpolate](https://github.com/Shanksir/CG/blob/master/Rasterization/images/Interpolate.png)


## Função drawTriangle()
A função recebe três pixels e desenha um triângulo. Basta chamar a função **drawLine()** três vezes.

```C
void drawTriangle(pixel_t p1, pixel_t p2, pixel_t p3) {
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, p1);
}
```

![Triangle](https://github.com/Shanksir/CG/blob/master/Rasterization/images/Triangle.png)


## Referências
- http://www.uobabylon.edu.iq/eprints/publication_2_22893_6215.pdf
- https://fr.wikipedia.org/wiki/Fichier:Bresenham_line.png
- ROGERS, D.F. Procedural Elements For Computer Graphics: 2. ed. Nova Iorque: McGraw-Hill, 1998
- Notas de Aula do Prof. Christian Pagot
