#include "top.hpp"

void dut(unsigned int numVert,
         unsigned int numEdge,
         unsigned int* offset,
         unsigned int* column,
         unsigned* color,
         unsigned* tmp0,
         unsigned* tmp1,
         unsigned* tmp2,
         unsigned* tmp3) {
    unsigned cnt = 0;
    for (int i = 0; i < numVert; i++) {
        color[i] = i;
    }
}
