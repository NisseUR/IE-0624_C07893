#include <pic14/pic12f683.h>


typedef unsigned int word;
word __at 0x2007 __CONFIG = (_WDT_OFF);

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

void main(void) {
    TRISIO = 0b00100000; // GPIO PIN5 In, the rest is Out 
    GPIO = 0b00000000; // all out 

    // Inicializa seed con un valor basado en una lectura del temporizador o un valor cambiante
    seed = TMR1;

    while (1) {
        if (GP5 == 0) { // pull down button
            if (GP5 == 0) { 
                if (index >= 10) {
                    reset(); // Reset the saved numbers after 10 numbers
                }
                int num;
                do {
                    seed++; // Cambia la semilla para cada número
                    num = seed % 100;
                } while (savedNumber(num));
                showNumber(num);
                save(num);
                while(GP5 == 0); // wait till button is not longer pushed
            }
        } else {
            showNumber(0); // show 00 while the button is not pushed 
        }
    }
}

void showNumber(int num) {
    units = num % 10;
    decimals = num / 10;

    for (int i = 0; i < 50; i++) {
        GPIO = decimal_digit[decimals];
        delay(1);  
        GPIO = unit_digit[units];
        delay(1);             
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
        delay(10); // Delay between blinks 
    }
}

void delay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++) {
        for (j = 0; j < 1275; j++);
    }
}