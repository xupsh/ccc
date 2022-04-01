#include "answer.hpp"

void minimum_spanning_tree(std::string xclbin_path, std::vector<CSRGraph> graph, std::vector<unsigned*>& mst) {
    xf::common::utils_sw::Logger logger(std::cout, std::cerr);
    cl_int fail;

    // platform related operations
    std::vector<cl::Device> devices = xcl::get_xil_devices();
    cl::Device device = devices[0];

    // Creating Context and Command Queue for selected Device
    cl::Context context(device, NULL, NULL, NULL, &fail);
    logger.logCreateContext(fail);
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &fail);
    logger.logCreateCommandQueue(fail);
    std::string devName = device.getInfo<CL_DEVICE_NAME>();
    printf("Found Device=%s\n", devName.c_str());

    cl::Program::Binaries xclBins = xcl::import_binary_file(xclbin_path);
    devices.resize(1);
    cl::Program program(context, devices, xclBins, NULL, &fail);
    logger.logCreateProgram(fail);
    cl::Kernel mst_kernel;
    mst_kernel = cl::Kernel(program, "mst_top", &fail);
    logger.logCreateKernel(fail);

    std::cout << "kernel has been created" << std::endl;

    for (int i = 0; i < 10; i++) {
        std::vector<cl_mem_ext_ptr_t> mext_o(4);
        mext_o[0] = {2, graph[i].offset, mst_kernel()};
        mext_o[1] = {3, graph[i].column, mst_kernel()};
        mext_o[2] = {4, graph[i].weight, mst_kernel()};
        mext_o[3] = {5, mst[i], mst_kernel()};

        // create device buffer and map dev buf to host buf
        cl::Buffer offset_buf, column_buf, weight_buf, mst_buf;
        offset_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(unsigned) * (graph[i].numVert + 1), &mext_o[0]);
        column_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(unsigned) * (graph[i].numEdge), &mext_o[1]);
        weight_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * (graph[i].numEdge), &mext_o[2]);
        mst_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                             sizeof(unsigned) * (graph[i].numVert), &mext_o[3]);

        std::vector<cl::Memory> ob_in;
        std::vector<cl::Memory> ob_out;
        std::vector<cl::Event> events_write(1);
        std::vector<cl::Event> events_kernel(1);
        std::vector<cl::Event> events_read(1);

        ob_in.push_back(offset_buf);
        ob_in.push_back(column_buf);
        ob_in.push_back(weight_buf);
        ob_in.push_back(mst_buf);
        q.enqueueMigrateMemObjects(ob_in, 0, nullptr, &events_write[0]);

        ob_out.push_back(mst_buf);

        // launch kernel and calculate kernel execution time
        std::cout << "kernel start------" << std::endl;
        int j = 0;
        mst_kernel.setArg(j++, graph[i].numVert);
        mst_kernel.setArg(j++, graph[i].numEdge);
        mst_kernel.setArg(j++, offset_buf);
        mst_kernel.setArg(j++, column_buf);
        mst_kernel.setArg(j++, weight_buf);
        mst_kernel.setArg(j++, mst_buf);
        q.enqueueTask(mst_kernel, &events_write, &events_kernel[0]);

        q.enqueueMigrateMemObjects(ob_out, 1, &events_kernel, &events_read[0]);
        q.finish();
    }
}
