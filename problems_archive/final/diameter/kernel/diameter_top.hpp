#include <iostream>

#define QUEUE_SIZE (1024 * 1024)

extern "C" void diameter_top(unsigned numVert,
                             unsigned numEdge,
                             unsigned* offset,
                             unsigned* column,
                             float* weight,
                             float* max_dist,
                             unsigned* src,
                             unsigned* des);
