#include "answer.hpp"
#include "utils.hpp"
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
    std::cout << "\n-------------------MST----------------\n";
    ArgParser parser(argc, argv);
    std::string tmpStr;
    if (!parser.getCmdOption("--xclbin", tmpStr)) {
        std::cout << "ERROR:xclbin path is not set!\n";
        return 1;
    }
    std::string xclbin_path = tmpStr;

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

    std::vector<unsigned*> mst(10);
    for (int i = 0; i < 10; i++) {
        mst[i] = aligned_alloc<unsigned>(graph[i].numVert);
        for (unsigned j = 0; j < graph[i].numVert; j++) {
            mst[i][j] = 0xFFFFFFFF;
        }
    }

    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);
    minimum_spanning_tree(xclbin_path, graph, mst);
    gettimeofday(&end_time, 0);
    std::cout << "User function execution time is: " << tvdiff(&start_time, &end_time) / 1000UL << "ms" << std::endl;

    unsigned err = 0;
    for (int k = 0; k < 10; k++) {
        unsigned numVertices = graph[k].numVert;
        //        unsigned numEdges = graph[k].numEdge;
        unsigned* offset32 = graph[k].offset;
        unsigned* column32 = graph[k].column;
        float* weight32 = graph[k].weight;
        unsigned* mst_tmp = mst[k];
        unsigned root = 0;
        float total = 0.0;
        std::set<unsigned long> tree;
        for (unsigned i = 0; i < numVertices; i++) {
            if (mst_tmp[i] == 0xFFFFFFFF) {
                std::cout << "vertex " << i << " not connected with the tree." << std::endl;
                err++;
            }
            unsigned start = offset32[mst_tmp[i]];
            unsigned end = offset32[mst_tmp[i] + 1];
            bool found = false;
            for (unsigned j = start; j < end; j++) {
                if (column32[j] == i) {
                    unsigned long tmp, left, right;
                    if (mst_tmp[i] != i) {
                        total = total + weight32[j];
                        left = mst_tmp[i];
                        right = i;
                        if (left >= right) {
                            tmp = 0UL | left << 32 | right;
                        } else {
                            tmp = 0UL | right << 32 | left;
                        }
                        tree.insert(tmp);
                    }
                    found = true;
                }
            }
            if (mst_tmp[i] == i) {
                found = true;
                root++;
            }
            if (!found) {
                err++;
                std::cout << "edge not found " << mst_tmp[i] << " " << i << std::endl;
            }
        }

        if (root > 1) {
            std::cout << "muliple root founded" << std::endl;
            err++;
        }
        if (tree.size() != numVertices - 1) {
            std::cout << "not a tree" << std::endl;
            err++;
        }

        std::cout << "MST weight is " << total << std::endl;
    }
    if (err == 0) {
        std::cout << "TEST PASS" << std::endl;
    } else {
        std::cout << "TEST FAIL" << std::endl;
    }
    return err;
}
