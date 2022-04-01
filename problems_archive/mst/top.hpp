#include <queue>
#include <set>
#include <iostream>

#define INTERFACE_MEMSIZE (100000)

class edge {
   public:
    unsigned int from;
    unsigned int to;
    float weight;
};

void dut(unsigned int numVert,
         unsigned int numEdge,
         unsigned int* offset,
         unsigned int* column,
         float* weight,
         unsigned* mst,
         unsigned* tmp0,
         unsigned* tmp1,
         unsigned* tmp2,
         unsigned* tmp3);
