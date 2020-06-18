#include "mygl.h"

void pixelInit(pixel_t *p, int x, int y, color_t color) {
    (*p).x = x;
    (*p).y = y;
    (*p).color = color;
}

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

int compare(color_t a, color_t b) {
    if(a.r == b.r && a.g == b.g && 
       a.b == b.b && a.a == b.a)
        return 1;
    else
        return 0;
}

color_t interpolate(color_t a, color_t b) {
    if(!compare(a, b)) {
        if(a.r > b.r)
            a.r--;
        else if(a.r < b.r)
            a.r++;

        if(a.g > b.g)
            a.g--;
        else if(a.g < b.g)
            a.g++;
        
        if(a.b > b.b)
            a.b--;
        else if(a.b < b.b)
            a.b++;
        
        if(a.a > b.a)
            a.a--;
        else if(a.a < b.a)
            a.a++;
    }
    
    return a;
}

void putPixel(pixel_t pixel) {
    int pos = pixel.x * 4 + pixel.y * IMAGE_WIDTH * 4;
    
    fb_ptr[pos++] = pixel.color.r;
    fb_ptr[pos++] = pixel.color.g;
    fb_ptr[pos++] = pixel.color.b;
    fb_ptr[pos]   = pixel.color.a;
}

void drawLine(pixel_t a, pixel_t b) {
    int dy = abs(b.y - a.y),
        dx = abs(b.x - a.x);
        
    int inverse = 0;
    
    if(!dx && !dy) {
        putPixel(a);
        
        return;
    }
    
    if(dy > dx) {   
        SWAP(pixel_t, a, b);
        SWAP(int, dx, dy);
        
        inverse = 1;
    }
    
    int incX = inc(b.x - a.x),
        incY = inc(b.y - a.y),
        d = 2*dy - dx;
    
    pixel_t p = {0, 0, a.color};
    
    putPixel(a);
    
    for(int x = a.x, y = a.y, i = 0; i < dx; putPixel(p), i++) {
        if(d < 0) {
            if(inverse)
                y += incY;
            else
                x += incX;
            
            d += 2*dy;
        }
        else {
            y += incY;
            x += incX;
            d += 2*(dy - dx);
        }
        
        pixelInit(&p, x, y, interpolate(p.color, b.color));
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
    /*pixel_t p1 = {  0,   0, {255, 0, 0, 255}},
            p2 = {256,   0, {255, 0, 0, 255}},
            p3 = {511,   0, {255, 0, 0, 255}},
            p4 = {  0, 256, {255, 0, 0, 255}},
            p5 = {256, 256, {255, 0, 0, 255}},
            p6 = {  0, 511, {255, 0, 0, 255}},
            p7 = {511, 256, {255, 0, 0, 255}},
            p8 = {511, 511, {255, 0, 0, 255}},
            p9 = {256, 511, {255, 0, 0, 255}};
    
    drawLine(p5, p1);
    drawLine(p5, p2);
    drawLine(p5, p3);
    drawLine(p5, p4);
    drawLine(p5, p6);
    drawLine(p5, p7);
    drawLine(p5, p8);
    drawLine(p5, p9);
    */
    
    pixel_t a = {256, 128, {255,   0,   0, 255}},
            b = {128, 383, {  0, 255,   0, 255}},
            c = {383, 383, {  0,   0, 255, 255}};
            
    drawTriangle(a, b ,c);
}
