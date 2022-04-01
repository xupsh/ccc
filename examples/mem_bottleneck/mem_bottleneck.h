
#ifndef MEM_BOTTLENECK_H_
#define MEM_BOTTLENECK_H_

#include <stdio.h>
#include "ap_cint.h"
#define N 128

typedef int din_t;
typedef int dout_t;

dout_t mem_bottleneck(din_t mem[N]);

#endif

