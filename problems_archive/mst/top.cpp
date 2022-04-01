#include "top.hpp"

bool operator<(const edge& e1, const edge& e2) {
    return e1.weight > e2.weight;
}

bool operator>(const edge& e1, const edge& e2) {
    return e1.weight < e2.weight;
}

std::vector<edge> prim(unsigned int allVert,
                       unsigned int allEdge,
                       unsigned int source,
                       unsigned int* offset,
                       unsigned int* column,
                       float* weight) {
    std::set<unsigned int> mstVert;
    std::vector<edge> mst;

    std::priority_queue<edge> q;

    mstVert.insert(source);
    for (int i = offset[source]; i < offset[source + 1]; i++) {
        edge tmp;
        tmp.from = source;
        tmp.to = column[i];
        tmp.weight = weight[i];
        q.push(tmp);
    }

    int cnt = 0;
    float totalweight = 0.0;
    while (!q.empty()) {
        edge tmp = q.top();
        q.pop();
        if (mstVert.find(tmp.to) == mstVert.end()) {
            mstVert.insert(tmp.to);
            totalweight = totalweight + tmp.weight;
            mst.push_back(tmp);
            for (int i = offset[tmp.to]; i < offset[tmp.to + 1]; i++) {
                edge out;
                out.from = tmp.to;
                out.to = column[i];
                out.weight = weight[i];
                if (mstVert.find(out.to) == mstVert.end()) q.push(out);
            }
        }
        cnt++;
    }

    return mst;
}

void dut(unsigned int numVert,
         unsigned int numEdge,
         unsigned int* offset,
         unsigned int* column,
         float* weight,
         unsigned* mst,
         unsigned* tmp0,
         unsigned* tmp1,
         unsigned* tmp2,
         unsigned* tmp3) {
    std::vector<edge> mst_prim;
    unsigned sourceID = 30;

    mst_prim = prim(numVert, numEdge, sourceID, offset, column, weight);

    for (int i = 0; i < numVert; i++) {
        mst[i] = 0xFFFFFFFF;
    }
    mst[sourceID] = sourceID;
    for (auto it = mst_prim.begin(); it != mst_prim.end(); it++) {
        mst[it->to] = it->from;
    }
}
