#ifndef MYGL_H
#define MYGL_H

#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)

#include <stdlib.h>
#include <math.h>
#include "core.h"
#include "frame_buffer.h"

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} color_t;

typedef struct pixel {
    int x;
    int y;
    color_t color;
} pixel_t;

void    pixelInit(pixel_t *, int, int, color_t);
int     inc(int);
double  dist(pixel_t, pixel_t);
color_t interpolate(pixel_t, pixel_t, pixel_t);


// Declaração da função que chamará as funções implementadas pelo aluno
void MyGlDraw(void);

void putPixel(pixel_t);
void drawLine(pixel_t, pixel_t);
void drawTriangle(pixel_t, pixel_t, pixel_t);


#endif  // MYGL_H
