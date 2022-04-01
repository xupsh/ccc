#include "utils.hpp"
#include "answer.hpp"

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

void load_data(std::string offsetfile, std::string columnfile, bool weighted, CSRGraph& graph) {
    char line[1024] = {0};
    int index = 0;

    std::fstream offsetfstream(offsetfile.c_str(), std::ios::in);
    if (!offsetfstream) {
        std::cout << "Error : " << offsetfile << " file doesn't exist !" << std::endl;
        exit(1);
    }

    offsetfstream.getline(line, sizeof(line));
    std::stringstream numOdata(line);
    numOdata >> graph.numVert;
    numOdata >> graph.numVert;

    graph.offset = aligned_alloc<unsigned>(graph.numVert + 1);
    while (offsetfstream.getline(line, sizeof(line))) {
        std::stringstream data(line);
        data >> graph.offset[index];
        index++;
    }
    offsetfstream.close();

    index = 0;
    std::fstream columnfstream(columnfile.c_str(), std::ios::in);
    if (!columnfstream) {
        std::cout << "Error : " << columnfile << " file doesn't exist !" << std::endl;
        exit(1);
    }

    columnfstream.getline(line, sizeof(line));
    std::stringstream numCdata(line);
    numCdata >> graph.numEdge;
    graph.column = aligned_alloc<unsigned>(graph.numEdge);
    graph.weighted = weighted;
    if (graph.weighted) {
        graph.weight = aligned_alloc<float>(graph.numEdge);
    }

    while (columnfstream.getline(line, sizeof(line))) {
        std::stringstream data(line);
        data >> graph.column[index];
        if (graph.weighted) {
            data >> graph.weight[index];
        }
        index++;
    }
    columnfstream.close();
}

void sssp_TB(unsigned int numVert,
             unsigned int numEdge,
             unsigned int source,
             unsigned int* offset,
             unsigned int* column,
             float* weight,
             float* distance) {
    for (unsigned i = 0; i < numVert; i++) {
        distance[i] = std::numeric_limits<float>::infinity();
    }

    std::queue<unsigned int> q;

    q.push(source);
    distance[source] = 0;
    while (!q.empty()) {
        unsigned int tmp = q.front();
        for (unsigned i = offset[tmp]; i < offset[tmp + 1]; i++) {
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
    std::cout << "\n-------------------estimated diameter----------------\n";
    // cmd parser
    ArgParser parser(argc, argv);
    std::string tmpStr;
    if (!parser.getCmdOption("--xclbin", tmpStr)) {
        std::cout << "ERROR:xclbin path is not set!\n";
        return 1;
    }
    std::string xclbin_path = tmpStr;

    std::string offsetfile;
    std::string columnfile;
    offsetfile = "data/small0-csr-offset.mtx";
    columnfile = "data/small0-csr-indicesweights.mtx";

    CSRGraph graphfile;
    load_data(offsetfile, columnfile, true, graphfile);
    std::cout << "file opened" << std::endl;

    std::vector<CSRGraph> graph(10);

    for (int i = 0; i < 10; i++) {
        graph[i] = graphfile;
    }

    std::vector<DiameterRes> diameter(10);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);
    estimated_diameter(xclbin_path, graph, diameter);
    gettimeofday(&end_time, 0);
    std::cout << "User function execution time is: " << tvdiff(&start_time, &end_time) / 1000UL << " ms" << std::endl;

    unsigned err = 0;

    for (int i = 0; i < 10; i++) {
        float* distance = aligned_alloc<float>(graph[i].numVert);
        sssp_TB(graph[i].numVert, graph[i].numEdge, diameter[i].src, graph[i].offset, graph[i].column, graph[i].weight,
                distance);
        if (std::fabs(distance[diameter[i].des] - diameter[i].diameter) / distance[diameter[i].des] > 0.0001) {
            std::cout << "source, destination, distance mismatch" << std::endl;
            err++;
        }
        if (distance[diameter[i].des] == std::numeric_limits<float>::infinity()) {
            std::cout << "not linked source destination found" << std::endl;
            err++;
        }
        std::cout << "source: " << diameter[i].src << " destination: " << diameter[i].des << std::endl;
        std::cout << "calculated diameter: " << diameter[i].diameter << std::endl;
    }
    if (err == 0) {
        std::cout << "TEST PASS" << std::endl;
    } else {
        std::cout << "TEST FAIL" << std::endl;
    }
    return err;
}
