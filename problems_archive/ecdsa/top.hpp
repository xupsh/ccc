#include "ecdsa.hpp"
#ifndef __SYNTHESIS__
#include <iostream>
#endif

void test(ap_uint<256> hash,
          ap_uint<256> k,
          ap_uint<256> privateKey,
          ap_uint<256>& Qx,
          ap_uint<256>& Qy,
          ap_uint<256>& r,
          ap_uint<256>& s,
          bool& kValid);

