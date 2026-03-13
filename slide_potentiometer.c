#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define WINDOW_SIZE 100
#define SLEEP_DUR 20
#define DEGREE 10

int main()
{
    stdio_init_all();

    adc_init();
    adc_gpio_init(26); // ADC0
    adc_gpio_init(27); // ADC1

    uint16_t window[WINDOW_SIZE] = {0};
    uint32_t window_sum = 0;
    int window_index = 0;
    int samples_collected = 0;

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
        adc_select_input(0);
        uint16_t adc0 = adc_read();

        adc_select_input(1);
        uint16_t adc1 = adc_read();

        uint16_t adc_avg = (adc0 + adc1) / 2;
        window_sum -= window[window_index];
        window[window_index] = adc_avg;
        window_sum += adc_avg;

        window_index = (window_index + 1) % WINDOW_SIZE;
        if (samples_collected < WINDOW_SIZE)
            samples_collected++;

        uint16_t rolling_avg = window_sum / samples_collected;

        printf("ADC Rolling Avg: %u\n", rolling_avg);
        double final_mm = 0;
        for (int i = 0; i <= DEGREE; i ++) {
            final_mm += coefficients[i] * pow(rolling_avg, i) / pow(10, -exponents[i]);
        }
        printf("FINAL MEASUREMENT (MM): %f\n", final_mm);
        sleep_ms(SLEEP_DUR);
    }
}