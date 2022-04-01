#include "top.hpp"

void dut(const int cols,
         hls::stream<DataType> sample_strm[8],
         hls::stream<bool>& e_sample_strm,
         hls::stream<ap_uint<512> >& weight_strm,
         hls::stream<bool>& eTag,
         hls::stream<ap_uint<1> >& predictionsStrm,
         hls::stream<bool>& predictionsTag) {
    xf::data_analytics::classification::svmPredict<DataType, dw, streamN, sampleD>(
        cols, sample_strm, e_sample_strm, weight_strm, eTag, predictionsStrm, predictionsTag);
}
