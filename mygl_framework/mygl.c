#include "mygl.h"

void pixelInit(pixel_t *p, int x, int y, color_t color) {
    (*p).x = x;
    (*p).y = y;
    (*p).color = color;
}

void putPixel(pixel_t pixel) {
    int pos = pixel.x * 4 + pixel.y * IMAGE_WIDTH * 4;
    
    fb_ptr[pos++] = pixel.color.red;
    fb_ptr[pos++] = pixel.color.green;
    fb_ptr[pos++] = pixel.color.blue;
    fb_ptr[pos]   = pixel.color.alpha;
}

void drawLine(pixel_t a, pixel_t b) {
    int dy = b.y - a.y,
        dx = b.x - a.x;
        
    int d = 2*dy - dx;
    
    pixel_t p;
    
    putPixel(a);
    
    for(int x = a.x, y = a.y; x < b.x; putPixel(p)) {
        if(d <= 0) {
            d += 2 * dy;
        }
        else {
            y++;
            d += 2 * (dy - dx);
        }
        
        x++;
        
        pixelInit(&p, x, y, a.color);
    }
    
    putPixel(b);
}

void drawTriangle(pixel_t p1, pixel_t p2, pixel_t p3) {
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, p1);
}

// Definição da função que chamará as funções implementadas pelo aluno
void MyGlDraw(void) {
    pixel_t p1 = {250, 250, {255, 0, 0, 255}},
            p2 = {200, 270, {255, 0, 0, 255}},
            p3 = {300, 270, {255, 0, 0, 255}};
            //p4 = {511, 511, {255, 255, 255, 255}};
    
    putPixel(p1);
    putPixel(p2);
    putPixel(p3);
    
    drawLine(p1, p2);
    drawLine(p1, p3);
    drawLine(p2, p3);
    
    //drawTriangle(p1, p2, p3);
}
