#include "top.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

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

    unsigned* offset32 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
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

    unsigned* column32 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    float* weight32 = aligned_alloc<float>(INTERFACE_MEMSIZE);
    while (columnfstream.getline(line, sizeof(line))) {
        std::stringstream data(line);
        data >> column32[index];
        data >> weight32[index];
        index++;
    }

    unsigned* mst = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);

    unsigned* tmp0 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp1 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp2 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp3 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);

    if (INTERFACE_MEMSIZE == 100000) {
        std::cout << "Warning: using default memory size (100000xsizeof(unsigned)) for tmp0, tmp1, tmp2, tmp3. Define "
                     "INTERFACE_MEMSIZE in the top.hpp for customize memory size."
                  << std::endl;
    }

    for (int i = 0; i < numVertices; i++) {
        mst[i] = -1;
    }

    dut(numVertices, numEdges, offset32, column32, weight32, mst, tmp0, tmp1, tmp2, tmp3);

    bool* edge_color = aligned_alloc<bool>(numEdges);
    memset(edge_color, 0, numEdges);

    unsigned err = 0;
    float total = 0.0;
    unsigned edge_cnt = 0;
    for (int i = 0; i < numVertices; i++) {
        if (mst[i] == -1) {
            std::cout << "vertex " << i << " not connected with the tree." << std::endl;
            err++;
        }
        unsigned start = offset32[mst[i]];
        unsigned end = offset32[mst[i] + 1];
        bool found = false;
        for (unsigned j = start; j < end; j++) {
            if (column32[j] == i) {
                if (mst[i] != i) {
                    total = total + weight32[j];
                }
                found = true;
                if (edge_color[j] == false && mst[i] != i) {
                    edge_color[j] = true;
                    edge_cnt++;
                }
            }
        }
        if (!found) {
            err++;
            std::cout << "edge not found " << mst[mst[i]] << " " << mst[i] << std::endl;
        }
    }

    if (edge_cnt != numVertices - 1) {
        err++;
    }
    std::cout << "f784c8c0 total mst weight: " << total << std::endl;

    return err;
}
