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

int main(int argc, const char* argv[]) {
    std::cout << "\n-------------------color----------------\n";
    // cmd parser
    ArgParser parser(argc, argv);
    std::string tmpStr;
    if (!parser.getCmdOption("--xclbin", tmpStr)) {
        std::cout << "ERROR:xclbin path is not set!\n";
        return 1;
    }
    std::string xclbin_path = tmpStr;

    if (!parser.getCmdOption("--scale", tmpStr)) {
        std::cout << "ERROR: scale is not set" << std::endl;
    }

    std::string offsetfile;
    std::string columnfile;
    offsetfile = "data/small0-undir-offset.mtx";
    columnfile = "data/small0-undir-indicesweights.mtx";

    CSRGraph graphfile;
    load_data(offsetfile, columnfile, true, graphfile);
    std::cout << "file opened" << std::endl;

    std::vector<CSRGraph> graph(10);

    for (int i = 0; i < 10; i++) {
        graph[i] = graphfile;
    }

    std::vector<unsigned*> color_res(10);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);
    color(xclbin_path, graph, color_res);
    gettimeofday(&end_time, 0);
    std::cout << "User function execution time is: " << tvdiff(&start_time, &end_time) / 1000UL << " ms" << std::endl;

    unsigned err = 0;

    for (int k = 0; k < 10; k++) {
        for (unsigned i = 0; i < graph[k].numVert; i++) {
            unsigned start = graph[k].offset[i];
            unsigned end = graph[k].offset[i + 1];
            for (unsigned j = start; j < end; j++) {
                if (i != graph[k].column[j] && color_res[k][i] == color_res[k][graph[k].column[j]]) err++;
            }
        }
        std::cout << "finish" << std::endl;

        std::set<unsigned> color_set;
        for (unsigned i = 0; i < graph[k].numVert; i++) {
            color_set.insert(color_res[k][i]);
        }
        std::cout << "color used " << color_set.size() << std::endl;
    }

    if (err == 0) {
        std::cout << "TEST PASSED" << std::endl;
    } else {
        std::cout << "TEST FAIL" << std::endl;
    }

    return err;
}
