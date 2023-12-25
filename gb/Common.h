#pragma once

#include <stdio.h>
 #include <cstdint>

 #define NO_IMPL {fprintf(stderr, "NO IMPLEMENTATION!"); exit(-5); }

 #define BIT(x, n) ((x & 1 << n) ? 1 : 0)
 #define BIT_SET(x, n, on) {if(on) {x |= 1 << n;} else { x &= ~(1 << n);}}