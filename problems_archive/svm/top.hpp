#include "svm_predict.hpp"
#include "table_sample.hpp"
typedef double DataType;
const int dw = sizeof(DataType) * 8;
const int streamN = 8;
const int sampleD = 4;
#define MAX_CLASS_BITS_ 8

void dut(const int cols,
         hls::stream<DataType> sample_strm[8],
         hls::stream<bool>& e_sample_strm,
         hls::stream<ap_uint<512> >& weight_strm,
         hls::stream<bool>& eTag,
         hls::stream<ap_uint<1> >& predictionsStrm,
         hls::stream<bool>& predictionsTag);
