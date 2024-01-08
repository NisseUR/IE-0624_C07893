#include <pic14/pic12f683.h>

typedef unsigned int word;
word __at 0x2007 __CONFIG = (_WDT_OFF & _MCLRE_OFF);

#define BUTTON GP5
#define COMMON_CATHODE GP4
#define BCD_PINS 0x0F // Los pines BCD están en los 4 bits menos significativos de GPIO

unsigned const char num[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

void delay(unsigned int tiempo) {
    unsigned int i, j;
    for (i = 0; i < tiempo; i++)
        for (j = 0; j < 1275; j++);
}

void main(void) {
    TRISIO = 0b00100000; // GP5 como entrada, el resto como salida
    GPIO = 0x00; // Todos los pines en cero

    while (1) {
        if (BUTTON == 0) { // Si el botón está presionado (pull-down)
            COMMON_CATHODE = 0; // Activar el cátodo común
            // Mostrar "0" en el display de las unidades
            GPIO = (GPIO & ~BCD_PINS) | num[0]; // Asegurar que solo los pines BCD cambien
            delay(5); // Retardo para el multiplexado
            COMMON_CATHODE = 1; // Desactivar el cátodo común
            
            COMMON_CATHODE = 0; // Activar el cátodo común
            // Mostrar "0" en el display de las decenas
            GPIO = (GPIO & ~BCD_PINS) | (num[0] << 4); // Asegurar que solo los pines BCD cambien
            delay(5); // Retardo para el multiplexado
            COMMON_CATHODE = 1; // Desactivar el cátodo común
        }
    }
}
