#include "mygl.h"

void putPixel(pixel_t pixel) {
    int pos = pixel.x * 4 + pixel.y * IMAGE_WIDTH * 4;
    
    fb_ptr[pos++] = pixel.color.red;
    fb_ptr[pos++] = pixel.color.green;
    fb_ptr[pos++] = pixel.color.blue;
    fb_ptr[pos]   = pixel.color.alpha;
}

/*
void drawTriangle(pixel_t p1, pixel_t p2, pixel_t p3) {
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, p1);
}
*/

// Definição da função que chamará as funções implementadas pelo aluno
void MyGlDraw(void) {
    pixel_t p1 = {  0,   0, {255, 255, 255, 255}},
            p2 = {511,   0, {255, 255, 255, 255}},
            p3 = {  0, 511, {255, 255, 255, 255}},
            p4 = {511, 511, {255, 255, 255, 255}};
    
    putPixel(p1);
    putPixel(p2);
    putPixel(p3);
    putPixel(p4);
}
