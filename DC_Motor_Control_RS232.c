// Controlar un motor de CD mediante la PC | Control a CD motor through the PC

// Librerias | Libraries
#include <16f877A.h>
#fuses XT, NOWDT, NOLVP, NOPROTECT
#use delay(clock = 4000000)
#use rs232(baud = 9600, xmit = pin_c6, rcv = pin_c7, bits = 8, parity = N)
#include <lcd.c>
#include <stdlib.h>

// Variables
char characters[3];
char c = '%';
int i = 0;
int16 speed;
float speedReaded, speedEngine;

// Funcion para imprimir en el LCD y en la PC | Function to print on the LCD and on the PC
void printText()
{
    // Mandar msj al LCD | Send msg to LCD
    lcd_gotoxy(1, 1);
    printf(lcd_putc, " .: Motor CD :.");
    lcd_gotoxy(1, 2);
    printf(lcd_putc, "Vel.=%3.0f%c ", speedReaded, c);

    // Mandar msj a la PC | Send msg to PC
    printf("\f .: Control de Velocidad de un Motor de CD Mediante La PC :.\n\n\r");
    printf(".: Ingrese la Velocidad Deseada de 0 a 100 i Presione Enter :.\n\n\r");
    printf("Velocidad = %3.0f%c\n\n\r", speedReaded, c);
    printf("Ingrese la Velocidad del Motor = ");
}

// Funcion de interrupcion | Interruption function
// int_ext: Interrupcion por flanco de bajada | Interruption by falling edge
#int_ext
void emergencyStop()
{
    i++;
}

// Funcion de interrupcion | Interruption function
// int_rda: Interrupcion por recepcion de datos | Interruption by data reception
#int_rda
void engine()
{
    // Leer el dato que se recibe | Read the data that is received
    gets(characters);
    // Convertir el dato a entero | Convert the data to integer
    speedReaded = atoi(characters);
    
    // Si el dato es mayor a 100, se le asigna 100 | If the data is greater than 100, 100 is assigned
    if (speedReaded > 100)
    {
        speedReaded = 100;
    }
    // Si el dato es menor a 0, se le asigna 0 | If the data is less than 0, 0 is assigned
    else if (speedReaded < 0)
    {
        speedReaded = 0;
    }

    // Multiplicar por 10 para que sea de 0 a 100 | Multiply by 10 to make it from 0 to 100
    speedEngine = (speedReaded) * (10);
    // Imprimir texto | Print text
    printText();
    // Si el dato es 0, el motor se detiene | If the data is 0, the motor stops
    i = 0;
}

// Funcion principal | Main function
void main()
{
    // Configuracion de la interrupcion | Interruption configuration
    // ext_int_edge (H_TO_L) es para flanco de bajada
    ext_int_edge(L_TO_H);
    // Habilitar interrupciones
    enable_interrupts(int_EXT);
    enable_interrupts(int_rda);
    enable_interrupts(global);

    // Configuracion del LCD | LCD configuration
    lcd_init();
    // Imprimir texto | Print text
    printText();

    // Ciclo infinito | Infinite cycle
    while (true)
    {
        // Si es igual a 0, el motor gira | If it is equal to 0, the motor rotates
        if (i == 0)
        {
            speed = speedEngine;

            output_high(PIN_B1);
            delay_ms(speed);
            output_low(PIN_B1);
            delay_ms(1000 - speed);
        }
        // Si es diferente de 0, el motor se detiene | If it is different from 0, the motor stops
        else
        {
            lcd_gotoxy(1, 2);
            printf(lcd_putc, "Motor Parado   ");
        }
    }
}
