#include <iostream>

#define INTERFACE_MEMSIZE (100000)

void dut(unsigned numVert,
         unsigned numEdge,
         unsigned* offset,
         unsigned* column,
         float* weight,
         float* max_dist,
         unsigned* src,
         unsigned* des,

         unsigned* tmp0,
         unsigned* tmp1,
         unsigned* tmp2,
         unsigned* tmp3);
