#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define WINDOW_SIZE 10

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
        sleep_ms(100);
    }
}