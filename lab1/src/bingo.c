#include <pic14/pic12f683.h>


typedef unsigned int word;
word __at 0x2007 __CONFIG = (_WDTE_OFF); // Desactiva el Watchdog Timer

// Definición de los pines
#define BUTTON GP5

unsigned const char unit_digit[] = {    
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};


unsigned const char decimal_digit[] = {    
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};

unsigned char saved[10] = {0};
unsigned char index = 0;
unsigned int seed = 0;
int units, decimals;

// Functions 
void delay(unsigned int time);
void showNumber(int num);
int savedNumber(int num);
void save(int num);
void reset(void);
void blink99(void);

void delay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++) {
        for (j = 0; j < 1275; j++);
    }
}

void main(void) {
    TRISIO = 0b00100000; // GPIO PIN5 In, the rest is Out 
    GPIO = 0b00000000; // all out 

    while (1) {
        if (BUTTON == 0) { // pull down button
            delay(1); // Debounce
            if (BUTTON == 0) { // 
                    if (index >= 10) {
                        reset(); // Reset the saved numbers after 10 numbers
                    }
                int num;
                do {
                    num = seed % 100;
                    seed++;
                } while (savedNumber(num));
                showNumber(num);
                save(num);
                while(BUTTON == 0); // wait till button is not longer pushed
            }
        } else {
            showNumber(0); // show 00 while the button is not pushed 
        }
        seed++;
    }
}

void showNumber(int num) {
    units = num % 10;
    decimals = num / 10;

    for (int i = 0; i < 100; i++) { // Incrementa este valor si es necesario
        // Asume que cuando GP4 es bajo, se seleccionan las decenas en el demux
        GP4 = 0; 
        GPIO = (decimal_digit[decimals]); // Muestra las decenas
        delay(1); // Retardo muy breve para la multiplexación

        // Asume que cuando GP4 es alto, se seleccionan las unidades en el demux
        GP4 = 1; 
        GPIO = (unit_digit[units]); // Muestra las unidades
        delay(1); // Retardo muy breve para la multiplexación
    }
}


int savedNumber(int num) {
    for (int i = 0; i < index; i++) {
        if (saved[i] == num) {
            return 1;
        }
    }
    return 0;
}

void save(int num) {
    saved[index] = num;
    index++;
    if (index >= 10) {
        index = 0; // reset index to let new numbers in 
    }
}

void reset(void) {
    for (int i = 0; i < 10; i++) {
        saved[i] = 0;
    }
    index = 0;
    blink99(); 
}


void blink99(void) {
    for (int i = 0; i < 3; i++) { // Blink 3 times
        showNumber(99);
        delay(2); // Delay between blinks 
    }
}