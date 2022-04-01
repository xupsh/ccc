#include <queue>
#include <set>
#include <iostream>

class edge {
   public:
    unsigned int from;
    unsigned int to;
    float weight;
};

extern "C" void mst_top(unsigned int numVert,
                        unsigned int numEdge,
                        unsigned int* offset,
                        unsigned int* column,
                        float* weight,
                        unsigned* mst);
