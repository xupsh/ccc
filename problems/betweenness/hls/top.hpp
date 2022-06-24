#define INTERFACE_MEMSIZE (100000)

extern "C" void dut(unsigned numVert,
                    unsigned numEdge,
                    unsigned *offset,
                    unsigned *column,
                    float *btwn,
                    unsigned *tmp0,
                    unsigned *tmp1,
                    unsigned *tmp2,
                    unsigned *tmp3);
