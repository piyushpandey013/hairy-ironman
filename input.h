
#ifndef INPUT_H_
#define INPUT_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint16_t adc_t; // 10-bit ADC storage

void initInput();

extern volatile adc_t adc_reading;

extern volatile bool adc_changed;

#endif
