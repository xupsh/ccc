#include "top.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <math.h>

#ifndef INTERFACE_MEMSIZE
#define INTERFACE_MEMSIZE (100000)
#endif

template <typename T>
T* aligned_alloc(std::size_t num) {
    void* ptr = NULL;

    if (posix_memalign(&ptr, 4096, num * sizeof(T))) throw std::bad_alloc();
    // ptr = (void*)malloc(num * sizeof(T));
    return reinterpret_cast<T*>(ptr);
}

void sssp_TB(unsigned int numVert,
             unsigned int numEdge,
             unsigned int source,
             unsigned int* offset,
             unsigned int* column,
             float* weight,
             float* distance) {
    for (int i = 0; i < numVert; i++) {
        distance[i] = std::numeric_limits<float>::infinity();
    }

    std::queue<unsigned int> q;

    q.push(source);
    distance[source] = 0;
    while (!q.empty()) {
        unsigned int tmp = q.front();
        for (int i = offset[tmp]; i < offset[tmp + 1]; i++) {
            float fromDist = distance[tmp];
            float toDist = distance[column[i]];
            float curDist = fromDist + weight[i];
            if (curDist < toDist) {
                distance[column[i]] = curDist;
                q.push(column[i]);
            }
        }
        q.pop();
    }
}

int main(int argc, const char* argv[]) {
    std::string offsetfile = "data-csr-offset.mtx";
    std::string columnfile = "data-csr-indicesweights.mtx";

    char line[1024] = {0};
    int index = 0;

    int numVertices;
    int numEdges;
    unsigned int sourceID = 30;

    std::fstream offsetfstream(offsetfile.c_str(), std::ios::in);
    if (!offsetfstream) {
        std::cout << "Error : " << offsetfile << " file doesn't exist !" << std::endl;
        exit(1);
    }

    offsetfstream.getline(line, sizeof(line));
    std::stringstream numOdata(line);
    numOdata >> numVertices;
    numOdata >> numVertices;

    unsigned* offset32 = aligned_alloc<unsigned>(numVertices + 1);
    while (offsetfstream.getline(line, sizeof(line))) {
        std::stringstream data(line);
        data >> offset32[index];
        index++;
    }

    std::fstream columnfstream(columnfile.c_str(), std::ios::in);
    if (!columnfstream) {
        std::cout << "Error : " << columnfile << " file doesn't exist !" << std::endl;
        exit(1);
    }

    index = 0;

    columnfstream.getline(line, sizeof(line));
    std::stringstream numCdata(line);
    numCdata >> numEdges;

    unsigned* column32 = aligned_alloc<unsigned>(numEdges);
    float* weight32 = aligned_alloc<float>(numEdges);
    while (columnfstream.getline(line, sizeof(line))) {
        std::stringstream data(line);
        data >> column32[index];
        data >> weight32[index];
        index++;
    }

    unsigned* tmp0 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp1 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp2 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp3 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);

    if (INTERFACE_MEMSIZE == 100000) {
        std::cout << "Warning: using default memory size (100000xsizeof(unsigned)) for tmp0, tmp1, tmp2, tmp3. Define "
                     "INTERFACE_MEMSIZE in the top.hpp for customize memory size."
                  << std::endl;
    }

    float max_dist[1];
    unsigned source[1];
    unsigned destination[1];
    dut(numVertices, numEdges, offset32, column32, weight32, max_dist, source, destination, tmp0, tmp1, tmp2, tmp3);
    unsigned err = 0;

    float* distance = aligned_alloc<float>(numVertices);
    sssp_TB(numVertices, numEdges, source[0], offset32, column32, weight32, distance);
    if (std::fabs(distance[destination[0]] - max_dist[0]) / distance[destination[0]] > 0.0001) {
        std::cout << "Wrong source, destination" << std::endl;
        err++;
    }
    std::cout << "source: " << source[0] << " destination: " << destination[0] << std::endl;
    std::cout << "435f8e47 calculated diameter: " << max_dist[0] << std::endl;

    return err;
}
