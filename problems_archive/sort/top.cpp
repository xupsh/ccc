/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "top.hpp"

void hls_db_insert_sort_function(hls::stream<DATA_TYPE>& din_strm,
                                 hls::stream<KEY_TYPE>& kin_strm,
                                 hls::stream<bool>& strm_in_end,
                                 hls::stream<DATA_TYPE>& dout_strm,
                                 hls::stream<KEY_TYPE>& kout_strm,
                                 hls::stream<bool>& strm_out_end,
                                 bool sign) {
    xf::database::insertSort<KEY_TYPE, DATA_TYPE, MaxSortNumber>(din_strm, kin_strm, strm_in_end, dout_strm, kout_strm,
                                                                 strm_out_end, sign);
}


