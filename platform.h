
#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>

/*
static const uint32_t f_cpu = 20000000;

static const uint32_t cpu_clocks_per_us = f_cpu / 1000000;
static const uint32_t cpu_clocks_per_ms = f_cpu / 1000;
*/
// C is annoying sometimes

#define f_cpu 20000000L
#define cpu_clocks_per_us f_cpu / 1000000
#define cpu_clocks_per_ms f_cpu / 1000

void usb_workaround(void);

#endif
