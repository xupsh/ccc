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

    unsigned* color = aligned_alloc<unsigned>(numVertices);

    unsigned* tmp0 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp1 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp2 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);
    unsigned* tmp3 = aligned_alloc<unsigned>(INTERFACE_MEMSIZE);

    if (INTERFACE_MEMSIZE == 100000) {
        std::cout << "Warning: using default memory size (100000xsizeof(unsigned)) for tmp0, tmp1, tmp2, tmp3. Define "
                     "INTERFACE_MEMSIZE in the top.hpp for customize memory size."
                  << std::endl;
    }
    dut(numVertices, numEdges, offset32, column32, color, tmp0, tmp1, tmp2, tmp3);
    unsigned err = 0;
    for (int i = 0; i < numVertices; i++) {
        unsigned start = offset32[i];
        unsigned end = offset32[i + 1];
        for (unsigned j = start; j < end; j++) {
            if (i != column32[j] && color[i] == color[column32[j]]) err++;
        }
    }
    std::cout << "finish" << std::endl;

    std::set<unsigned> color_set;
    for (int i = 0; i < numVertices; i++) {
        color_set.insert(color[i]);
    }
    std::cout << "fb5a6b9b color used " << color_set.size() << std::endl;

    return err;
}
