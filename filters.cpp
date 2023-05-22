#include "stdafx.h"
#include "filters.h"
#include "ppm.h"

#ifndef FILTERS_CPP
#define FILTERS_CPP

#define BLACK 0

using namespace std;

void plain(PPM& imagen, unsigned char intensidad){
    for(int fila = 0; fila < imagen.alto; fila++) {
        for(int columna = 0; columna < imagen.ancho; columna++) {
            imagen.setPixel(fila, columna, Pixel(intensidad,intensidad,intensidad));
        }
    }
}

void blackWhite(PPM &imagen) {
    for (int fila = 0; fila < imagen.alto; fila++) {
        for (int columna = 0; columna < imagen.ancho; columna++) {
            short int promedio = imagen.getPixel(fila, columna).sumaCumulativa()/3;
            Pixel pixel_final(promedio, promedio, promedio);
            imagen.setPixel(fila, columna, pixel_final);
        }
    }
}

void shades(PPM &imagen, unsigned char cantidad_grises) {
    for (int fila = 0; fila < imagen.alto; fila++) {
        for (int columna = 0; columna < imagen.ancho; columna++) {
            short int promedio = imagen.getPixel(fila, columna).sumaCumulativa()/3;
            short int valor_por_grado = 256/(cantidad_grises-1);
            short int valor_normalizado = (static_cast<short int>(promedio/valor_por_grado)) * valor_por_grado;
            Pixel pixel_final(valor_normalizado, valor_normalizado, valor_normalizado);
            imagen.setPixel(fila, columna, pixel_final);
        }
    }
}

void brightness(PPM &imagen, float porcentaje_brillo, int inicio, int fin) {
    for (int fila = 0; fila < imagen.alto; fila++) {
        for (int columna = 0; columna < imagen.ancho; columna++) {
            Pixel pixel_final = imagen.getPixel(fila, columna);
            pixel_final.sumar(255.f*porcentaje_brillo);
            imagen.setPixel(fila, columna, pixel_final.truncar());
        }
    }
}

void contrast(PPM &imagen, float contraste) {
    float factor_intensidad = (259.f * (contraste + 255.f)) / (255.f * (259.f - contraste));
    auto calculo_intensidad = [factor_intensidad] (unsigned char valor_color) -> short int { return static_cast<short int>(factor_intensidad*(valor_color-128)+128); };
    for (int fila = 0; fila < imagen.alto; fila++) {
        for (int columna = 0; columna < imagen.ancho; columna++) {
            Pixel pixel_final = imagen.getPixel(fila, columna);
            pixel_final.rojo = calculo_intensidad(pixel_final.rojo);
            pixel_final.verde = calculo_intensidad(pixel_final.verde);
            pixel_final.azul = calculo_intensidad(pixel_final.azul);
            imagen.setPixel(fila, columna, pixel_final.truncar());
        }
    }
}

void merge(PPM &primera_imagen, PPM &segunda_imagen, float alfa_primera_imagen) {
    float alfa_segunda_imagen = 1.f - alfa_primera_imagen;
    for (int fila = 0; fila < primera_imagen.alto; fila++) {
        for (int columna = 0; columna < primera_imagen.ancho; columna++) {
            Pixel pixel_primera_imagen = primera_imagen.getPixel(fila, columna);
            Pixel pixel_segunda_imagen = segunda_imagen.getPixel(fila, columna);

            pixel_primera_imagen.multiplicar(alfa_primera_imagen);
            pixel_segunda_imagen.multiplicar(alfa_segunda_imagen);
            pixel_primera_imagen.sumarPixel(pixel_segunda_imagen);
            
            primera_imagen.setPixel(fila, columna, pixel_primera_imagen);
        }
    }
}

#endif