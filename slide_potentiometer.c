#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main()
{
    stdio_init_all();

    adc_init();
    adc_gpio_init(26); // ADC0
    adc_gpio_init(27); // ADC1

    while (true) {
        adc_select_input(0);
        uint16_t adc0 = adc_read();

        adc_select_input(1);
        uint16_t adc1 = adc_read();

        uint16_t adc_avg = (adc0 + adc1) / 2;

        printf("ADC: %u\n", adc_avg);
        sleep_ms(100);
    }
}
