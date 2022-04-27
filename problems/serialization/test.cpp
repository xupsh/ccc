#include <string>
#include <fstream>
#include <iostream>
#include "top.hpp"

using namespace std;
int main() {
    const char* input_filename = "mix.obj";
    const char* golden_filename = "mix.arrow";

    // read in golden
    ifstream goldenfile(golden_filename, ios::binary);
    if (!goldenfile) {
        std::cout << "ERROR: golden file open failed." << std::endl;
        return 1;
    }

    goldenfile.seekg(0, goldenfile.end);
    int golden_sz = goldenfile.tellg();
    goldenfile.seekg(0, goldenfile.beg);

    char* golden_buff = new char[golden_sz];
    goldenfile.read(golden_buff, golden_sz);
    if (goldenfile) {
        std::cout << "INFO: golden file " << golden_sz << " bytes read successfully." << std::endl;
    } else {
        std::cout << "ERROR: golden read failed." << std::endl;
        delete[] golden_buff;
        return 1;
    }

    goldenfile.close();

    // read in input
    ifstream inputfile(input_filename, ios::in | ios::binary);
    if (!inputfile) {
        std::cout << "ERROR: input file open failed." << std::endl;
        delete[] golden_buff;
        return 1;
    }

    inputfile.seekg(0, inputfile.end);
    int input_sz = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);

    char* input_buff = new char[input_sz];
    inputfile.read(input_buff, input_sz);
    if (inputfile) {
        std::cout << "INFO: input file " << input_sz << " bytes read successfully." << std::endl;
    } else {
        std::cout << "ERROR: input read failed." << std::endl;
        delete[] golden_buff;
        delete[] input_buff;
        return 1;
    }
    inputfile.close();

    // allocate output buff
    char* output_buff = new char[DST_INTERFACE_SZ];

    // dut
    std::cout << "INFO: dut running..." << std::endl;
    dut(input_buff, input_sz, output_buff);

    // result check
    std::cout << "INFO: result check running..." << std::endl;
    int err = 0;
    for (int i = 0; i < golden_sz; ++i) {
        if (golden_buff[i] != output_buff[i]) {
            err++;
        }
    }

    if (err == 0) {
        std::cout << "INFO: ========= Test PASS ========" << std::endl;

    } else {
        std::cout << "INFO: ========= Test FAIL ========" << std::endl;
        //return err;
    }
    delete[] golden_buff;
    delete[] input_buff;
    delete[] output_buff;
    
    
    
    
    
    const char* input_filename1 = "fix.obj";
    const char* golden_filename1 = "fix.arrow";

    // read in golden
    ifstream goldenfile1(golden_filename1, ios::binary);
    if (!goldenfile1) {
        std::cout << "ERROR: golden file open failed." << std::endl;
        return 1;
    }

    goldenfile1.seekg(0, goldenfile1.end);
    int golden_sz1 = goldenfile1.tellg();
    goldenfile1.seekg(0, goldenfile1.beg);

    char* golden_buff1 = new char[golden_sz1];
    goldenfile1.read(golden_buff1, golden_sz1);
    if (goldenfile1) {
        std::cout << "INFO: golden file " << golden_sz1 << " bytes read successfully." << std::endl;
    } else {
        std::cout << "ERROR: golden read failed." << std::endl;
        delete[] golden_buff1;
        return 1;
    }

    goldenfile1.close();

    // read in input
    ifstream inputfile1(input_filename1, ios::in | ios::binary);
    if (!inputfile1) {
        std::cout << "ERROR: input file open failed." << std::endl;
        delete[] golden_buff1;
        return 1;
    }

    inputfile1.seekg(0, inputfile1.end);
    int input_sz1 = inputfile1.tellg();
    inputfile1.seekg(0, inputfile1.beg);

    char* input_buff1 = new char[input_sz1];
    inputfile1.read(input_buff1, input_sz1);
    if (inputfile1) {
        std::cout << "INFO: input file " << input_sz1 << " bytes read successfully." << std::endl;
    } else {
        std::cout << "ERROR: input read failed." << std::endl;
        delete[] golden_buff1;
        delete[] input_buff1;
        return 1;
    }
    inputfile1.close();

    // allocate output buff
    char* output_buff1 = new char[DST_INTERFACE_SZ];

    // dut
    std::cout << "INFO: dut running..." << std::endl;
    dut(input_buff1, input_sz1, output_buff1);

    // result check
    std::cout << "INFO: result check running..." << std::endl;
    int err1 = 0;
    for (int i = 0; i < golden_sz1; ++i) {
        if (golden_buff1[i] != output_buff1[i]) {
            err1++;
        }
    }

    if (err1 == 0) {
        std::cout << "INFO: ========= Test PASS ========" << std::endl;

    } else {
        std::cout << "INFO: ========= Test FAIL ========" << std::endl;
        //return err;
    }
    delete[] golden_buff1;
    delete[] input_buff1;
    delete[] output_buff1;
    
    
    
    
    
    const char* input_filename2 = "var.obj";
    const char* golden_filename2 = "var.arrow";

    // read in golden
    ifstream goldenfile2(golden_filename2, ios::binary);
    if (!goldenfile2) {
        std::cout << "ERROR: golden file open failed." << std::endl;
        return 1;
    }

    goldenfile2.seekg(0, goldenfile2.end);
    int golden_sz2 = goldenfile2.tellg();
    goldenfile2.seekg(0, goldenfile2.beg);

    char* golden_buff2 = new char[golden_sz2];
    goldenfile2.read(golden_buff2, golden_sz2);
    if (goldenfile2) {
        std::cout << "INFO: golden file " << golden_sz2 << " bytes read successfully." << std::endl;
    } else {
        std::cout << "ERROR: golden read failed." << std::endl;
        delete[] golden_buff2;
        return 1;
    }

    goldenfile2.close();

    // read in input
    ifstream inputfile2(input_filename2, ios::in | ios::binary);
    if (!inputfile2) {
        std::cout << "ERROR: input file open failed." << std::endl;
        delete[] golden_buff2;
        return 1;
    }

    inputfile2.seekg(0, inputfile2.end);
    int input_sz2 = inputfile2.tellg();
    inputfile2.seekg(0, inputfile2.beg);

    char* input_buff2 = new char[input_sz2];
    inputfile2.read(input_buff2, input_sz2);
    if (inputfile2) {
        std::cout << "INFO: input file " << input_sz2 << " bytes read successfully." << std::endl;
    } else {
        std::cout << "ERROR: input read failed." << std::endl;
        delete[] golden_buff2;
        delete[] input_buff2;
        return 1;
    }
    inputfile2.close();

    // allocate output buff
    char* output_buff2 = new char[DST_INTERFACE_SZ];

    // dut
    std::cout << "INFO: dut running..." << std::endl;
    dut(input_buff2, input_sz2, output_buff2);

    // result check
    std::cout << "INFO: result check running..." << std::endl;
    int err2 = 0;
    for (int i = 0; i < golden_sz2; ++i) {
        if (golden_buff2[i] != output_buff2[i]) {
            err2++;
        }
    }

    if (err == 0) {
        std::cout << "INFO: ========= Test PASS ========" << std::endl;

    } else {
        std::cout << "INFO: ========= Test FAIL ========" << std::endl;
    }
    delete[] golden_buff2;
    delete[] input_buff2;
    delete[] output_buff2;
    
    return err;
}