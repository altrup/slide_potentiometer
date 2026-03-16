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

    double coefficients[DEGREE + 1] = {
        -1.57070636199206, 
        9.099533214, 
        6.57630427575213, 
        -2.20152600185113,
        8.67185250112384, 
        -1.68566364368486, 
        1.91483726221163, 
        -1.32885864307038, 
        5.55171762743316, 
        -1.28352125540291, 
        1.2621127968903
    };
    int exponents[DEGREE + 1] = {0, -2, -5, -6, -9, -11, -14, -17, -21, -24, -28};

    while (true) {
        printf("Press Enter to Continue\n");
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