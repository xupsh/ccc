#include "top.hpp"
#include <list>
#include <queue>
#include <stack>

void dut(
        unsigned numVert, 
        unsigned numEdge, 
        unsigned *offset, 
        unsigned *column,
        float *btwn, 
        unsigned *tmp0, 
        unsigned *tmp1, 
        unsigned *tmp2,
        unsigned *tmp3) {
  // clang-format off

    const unsigned MEMSIZE=INTERFACE_MEMSIZE;
#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem0 port = offset depth = MEMSIZE

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem1 port = column depth = MEMSIZE

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem3 port = btwn depth = MEMSIZE

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem6 port = tmp0 depth = MEMSIZE

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem7 port = tmp1 depth = MEMSIZE

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem8 port = tmp2 depth = MEMSIZE

#pragma HLS INTERFACE m_axi offset = slave latency = 32 num_write_outstanding = 1 num_read_outstanding = \
    16 max_write_burst_length = 2 max_read_burst_length = 256 bundle = gmem9 port = tmp3 depth = MEMSIZE

// clang-format on
#ifndef __SYNTHESIS__
  for (int i = 0; i < numVert; i++) {
    btwn[i] = 0;
  }
  for (int i = 0; i < numVert; i++) {
    std::stack<unsigned> s;
    std::vector<std::list<unsigned>> p(numVert);
    std::vector<float> sigma(numVert);
    std::vector<int> dist(numVert);
    std::queue<unsigned> q;
    unsigned source = i;

    for (int j = 0; j < numVert; j++) {
      sigma[j] = 0;
      dist[j] = -1;
    }
    sigma[source] = 1;
    dist[source] = 0;

    q.push(source);
    while (!q.empty()) {
      unsigned v = q.front();
      s.push(v);
      for (int j = offset[v]; j < offset[v + 1]; j++) {
        unsigned w = column[j];
        if (dist[w] < 0) {
          q.push(w);
          dist[w] = dist[v] + 1;
        }
        if (dist[w] == dist[v] + 1) {
          sigma[w] = sigma[w] + sigma[v];
          p[w].push_back(v);
        }
      }
      q.pop();
    }

    std::vector<float> delta(numVert);
    for (int j = 0; j < numVert; j++) {
      delta[j] = 0;
    }
    while (!s.empty()) {
      unsigned w = s.top();
      if (source != w) {
        btwn[w] = btwn[w] + delta[w];
      }
      for (std::list<unsigned>::iterator it = p[w].begin(); it != p[w].end();
           it++) {
        unsigned v = *it;
        delta[v] = delta[v] + (sigma[v] / sigma[w]) * (1 + delta[w]);
        // if (source != w) {
        //     btwn[w] = btwn[w] + delta[w];
        // }
      }
      s.pop();
    }
  }
#else
  for (int i = 0; i < numVert; i++) {
    btwn[i] = i;
  }
#endif
}
