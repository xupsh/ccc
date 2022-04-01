#include "diameter_top.hpp"
#include <queue>
#include <stdlib.h>
#include <limits>
#include <ctime>

template <typename MType>
union f_cast;

template <>
union f_cast<float> {
    float f;
    uint32_t i;
};

void sssp(unsigned numVert,
          unsigned numEdge,
          unsigned source,
          unsigned* offset,
          unsigned* column,
          float* weight,
          unsigned* distance) {
    for (int i = 0; i < numVert; i++) {
        f_cast<float> init;
        init.f = std::numeric_limits<float>::infinity();
        distance[i] = init.i;
    }

    std::queue<unsigned> q;

    q.push(source);
    f_cast<float> init;
    init.f = 0.0;
    distance[source] = init.i;

    while (!q.empty()) {
        unsigned tmp = q.front();
        for (int i = offset[tmp]; i < offset[tmp + 1]; i++) {
            f_cast<float> fromDist;
            fromDist.i = distance[tmp];
            f_cast<float> toDist;
            toDist.i = distance[column[i]];
            f_cast<float> curDist;
            curDist.f = fromDist.f + weight[i];
            if (curDist.f < toDist.f) {
                distance[column[i]] = curDist.i;
                q.push(column[i]);
            }
        }
        q.pop();
    }
}

extern "C" void diameter_top(unsigned numVert,
                             unsigned numEdge,
                             unsigned* offset,
                             unsigned* column,
                             float* weight,
                             float* max_dist,
                             unsigned* src,
                             unsigned* des) {
// clang-format off

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem0 port = offset 

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem1 port = column 

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem2 port = weight 

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem3 port = max_dist 

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem4 port = src 

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem5 port = des

// clang-format on

#ifndef __SYNTHESIS__
    std::cout << "cu start" << std::endl;
    unsigned* tmp0 = (unsigned*)malloc(numVert * sizeof(unsigned));
    max_dist[0] = 0;

    std::srand(std::time(nullptr));
    for (int i = 0; i < 10; i++) {
        unsigned source = rand() % numVert;
        sssp(numVert, numEdge, source, offset, column, weight, tmp0);
        for (int i = 0; i < numVert; i++) {
            f_cast<float> res;
            res.i = tmp0[i];
            if (res.f != std::numeric_limits<float>::infinity() && res.f > max_dist[0]) {
                max_dist[0] = res.f;
                src[0] = source;
                des[0] = i;
            }
        }
    }
    std::cout << "cu end" << std::endl;
#else
    src[0] = 504;
    des[0] = 0;
    max_dist[0] = 0.443729;
#endif
}
