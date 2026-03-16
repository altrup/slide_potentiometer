#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/adc.h"

#define DEGREE 10

int main()
{
    stdio_init_all();

    adc_init();
    adc_gpio_init(26); // ADC0

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("PRESS ENTER TO MEASURE\n===============\n\n");

    double coefficients[DEGREE + 1] = {
        -1.32977233546821, 
        1.86828161394947, 
        -1.15826279134924, 
        4.32103447181343,
        -9.80170998379058, 
        1.41407593127346, 
        -1.31732979583333, 
        7.8591849744915, 
        -2.88850498506777, 
        5.9350558071851, 
        -5.19949058137809
    };
    int exponents[DEGREE + 1] = {0, -1, -3, -6, -9, -11, -14, -18, -21, -25, -29};

    while (true) {
        getchar(); // wait for Enter

        adc_select_input(0);
        uint16_t adc = adc_read();

        printf("ADC: %u\n", adc);
        double final_mm = 0;
        for (int i = 0; i <= DEGREE; i ++) {
            final_mm += coefficients[i] * pow(adc, i) / pow(10, -exponents[i]);
        }
        printf("FINAL MEASUREMENT (MM): %f\n\n", final_mm);
    }
}