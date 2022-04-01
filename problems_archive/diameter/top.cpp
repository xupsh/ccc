#include "top.hpp"
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
         unsigned* tmp3) {
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
    /*    src[0] = 0;
        des[0] = 1;
        max_dist[0] = 10.0;*/
}
