#ifndef MYGL_H
#define MYGL_H

#include "core.h"
#include "frame_buffer.h"

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} color_t;

typedef struct pixel {
    int x;
    int y;
    color_t color;
} pixel_t;


// Declaração da função que chamará as funções implementadas pelo aluno
void MyGlDraw(void);

void putPixel(pixel_t);
// void drawTriangle(pixel_t, pixel_t, pixel_t);


#endif  // MYGL_H
