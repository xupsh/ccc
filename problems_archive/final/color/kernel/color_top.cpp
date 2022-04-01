#include "color_top.hpp"

extern "C" void color_top(unsigned numVert, unsigned numEdge, unsigned* offset, unsigned* column, unsigned* color) {
// clang-format off

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem0 port = offset

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem1 port = column

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem2 port = color

    // clang-format on

    unsigned cnt = 0;
    for (int i = 0; i < numVert; i++) {
        color[i] = i;
    }
}
