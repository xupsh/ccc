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

template <typename Key_Type>
void generate_test_data(uint64_t Number, std::vector<Key_Type>& testVector) {
    srand(1);
    for (int i = 0; i < Number; i++) {
        testVector.push_back(rand()); // generate random key value
    }
    std::cout << " random test data generated! " << std::endl;
}

template <typename Data_Type, typename Key_Type>
void reference_sort(hls::stream<Key_Type>& ref_in_strm,
                    hls::stream<Key_Type>& ref_out_strm,
                    uint64_t Test_Number,
                    uint64_t Max_Sort_Number,
                    bool sign) {
    Key_Type key_strm[Max_Sort_Number];

    for (int64_t j = Test_Number; j >= 0; j = j - Max_Sort_Number) {
        // determine sort number for std::sort
        if (j >= Max_Sort_Number) {
            for (int i = 0; i < 1024; i++) {
                key_strm[i] = ref_in_strm.read();
            }

            std::sort(&key_strm[0], &key_strm[Max_Sort_Number]);

            if (sign) {
                for (int i = 0; i < 1024; i++) {
                    ref_out_strm.write(key_strm[i]);
                }
            } else {
                for (int i = 1024 - 1; i >= 0; i--) {
                    ref_out_strm.write(key_strm[i]);
                }
            }
        } else {
            for (int i = 0; i < j; i++) {
                key_strm[i] = ref_in_strm.read();
            }

            std::sort(&key_strm[0], &key_strm[j]);

            if (sign) {
                for (int i = 0; i < j; i++) {
                    ref_out_strm.write(key_strm[i]);
                }
            } else {
                for (int i = j - 1; i >= 0; i--) {
                    ref_out_strm.write(key_strm[i]);
                }
            }
        }
    }
}



int main() {
    int i;

    /// sort_strm_test

    std::vector<uint32_t> testVector;
    hls::stream<bool> din_strm_end("din_strm_end");
    hls::stream<bool> dout_strm_end("dout_strm_end");
    hls::stream<uint32_t> din_strm("din_strm");
    hls::stream<uint32_t> dout_strm("dout_strm");
    hls::stream<uint32_t> kin_strm("kin_strm");
    hls::stream<uint32_t> kout_strm("kout_strm");

    hls::stream<uint32_t> ref_kin_strm("ref_kin_strm");
    hls::stream<uint32_t> ref_kout_strm("ref_kout_strm");
    hls::stream<uint32_t> ref_din_strm("ref_din_strm");
    hls::stream<uint32_t> ref_dout_strm("ref_dout_strm");

    int nerror = 0;

    // generate test data
    generate_test_data<uint32_t>(4044, testVector);

    // prepare input data
    std::cout << "testVector List:" << std::endl;
    for (std::string::size_type i = 0; i < 4044; i++) {
        // print vector value
        std::cout << "Index=" << i << ' ';
        std::cout << "Vector=" << testVector[i] << std::endl;

        kin_strm.write(testVector[i]);     // write data to kin_strm
        din_strm.write(testVector[i]);     // write data to din_strm
        ref_kin_strm.write(testVector[i]); // write data to ref_kin_strm
        din_strm_end.write(false);         // write data to end_flag_strm
    }
    din_strm_end.write(true);

    // call insert_sort function

    hls_db_insert_sort_function(din_strm, kin_strm, din_strm_end, dout_strm, kout_strm, dout_strm_end, SORT_ASCENDING);

    // run reference sort
    reference_sort<uint32_t, uint32_t>(ref_kin_strm, ref_kout_strm, 4044, 1024, SORT_ASCENDING);

    //===== check if the output flag e_out_strm is correct or not =====
    for (int i = 0; i < 4044; i++) {
        bool e = dout_strm_end.read();
        if (e) {
            std::cout << "\nthe output flag is incorrect" << std::endl;
            nerror++;
        }
    }
    // read out the last flag that e should =1
    bool e = dout_strm_end.read();
    if (!e) {
        std::cout << "\nthe last output flag is incorrect" << std::endl;
        nerror++;
    }

    uint32_t key, ref_key;
    uint32_t data, ref_data;
    for (int i = 0; i < 4044; i++) {
        // compare the key
        key = kout_strm.read();
        data = dout_strm.read();
        ref_key = ref_kout_strm.read();
        ref_data = ref_key;
        // std::cout << "Index=" << i <<' '<< "key=" << key <<' '<< "reference key=" << ref_key<< std::endl;
        bool cmp_key = (key == ref_key) ? 1 : 0;
        bool cmp_data = (data == ref_data) ? 1 : 0;
        if (!cmp_key || !cmp_data) {
            nerror++;
            std::cout << "Index=" << i << ' ' << "key=" << key << ' ' << "reference key=" << ref_key << ' '
                      << "data=" << data << ' ' << "reference data=" << ref_data;
            std::cout << "\nthe sort key is incorrect" << std::endl;
        }
    }

    // print result
    if (nerror) {
        std::cout << "\nFAIL: nerror= " << nerror << " errors found.\n";
    } else {
        std::cout << "\nPASS: no error found.\n";
    }
    return 0;
}
