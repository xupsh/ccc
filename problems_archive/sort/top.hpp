#include <vector> // std::vector
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <algorithm>

#include "insert_sort.hpp"

typedef uint32_t KEY_TYPE;
typedef uint32_t DATA_TYPE;

/// @brief basic sort operators
enum InsertSortOrder { SORT_ASCENDING = 1, SORT_DESCENDING = 0 };

#define MaxSortNumber 1024
#define TestNumber 4044
#define OP SORT_ASCENDING

void hls_db_insert_sort_function(hls::stream<DATA_TYPE>& din_strm,
                                 hls::stream<KEY_TYPE>& kin_strm,
                                 hls::stream<bool>& strm_in_end,
                                 hls::stream<DATA_TYPE>& dout_strm,
                                 hls::stream<KEY_TYPE>& kout_strm,
                                 hls::stream<bool>& strm_out_end,
                                 bool sign);
