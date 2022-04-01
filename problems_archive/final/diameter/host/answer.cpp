#include "answer.hpp"

void estimated_diameter(std::string xclbin_path, std::vector<CSRGraph> graph, std::vector<DiameterRes>& res) {
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
    cl::Kernel diameter;
    diameter = cl::Kernel(program, "diameter_top", &fail);
    logger.logCreateKernel(fail);

    std::cout << "kernel has been created" << std::endl;

    for (int i = 0; i < 10; i++) {
        float* max_dist_tmp = aligned_alloc<float>(1 * sizeof(float));
        unsigned* source_tmp = aligned_alloc<unsigned>(1 * sizeof(unsigned));
        unsigned* destination_tmp = aligned_alloc<unsigned>(1 * sizeof(unsigned));

        std::vector<cl_mem_ext_ptr_t> mext_o(6);
        mext_o[0] = {2, graph[i].offset, diameter()};
        mext_o[1] = {3, graph[i].column, diameter()};
        mext_o[2] = {4, graph[i].weight, diameter()};
        mext_o[3] = {5, max_dist_tmp, diameter()};
        mext_o[4] = {6, source_tmp, diameter()};
        mext_o[5] = {7, destination_tmp, diameter()};

        // create device buffer and map dev buf to host buf
        cl::Buffer offset_buf, column_buf, weight_buf, max_dist_buf, source_buf, destination_buf;
        offset_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(unsigned) * (graph[i].numVert + 1), &mext_o[0]);
        column_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(unsigned) * (graph[i].numEdge), &mext_o[1]);
        weight_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * (graph[i].numEdge), &mext_o[2]);
        max_dist_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                  sizeof(unsigned) * (1), &mext_o[3]);
        source_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(unsigned) * (1), &mext_o[4]);
        destination_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                     sizeof(unsigned) * (1), &mext_o[5]);

        std::vector<cl::Memory> ob_in;
        std::vector<cl::Memory> ob_out;
        std::vector<cl::Event> events_write(1);
        std::vector<cl::Event> events_kernel(1);
        std::vector<cl::Event> events_read(1);

        ob_in.push_back(offset_buf);
        ob_in.push_back(column_buf);
        ob_in.push_back(weight_buf);
        q.enqueueMigrateMemObjects(ob_in, 0, nullptr, &events_write[0]);

        ob_out.push_back(max_dist_buf);
        ob_out.push_back(source_buf);
        ob_out.push_back(destination_buf);

        // launch kernel and calculate kernel execution time
        int j = 0;
        diameter.setArg(j++, graph[i].numVert);
        diameter.setArg(j++, graph[i].numEdge);
        diameter.setArg(j++, offset_buf);
        diameter.setArg(j++, column_buf);
        diameter.setArg(j++, weight_buf);
        diameter.setArg(j++, max_dist_buf);
        diameter.setArg(j++, source_buf);
        diameter.setArg(j++, destination_buf);

        q.enqueueTask(diameter, &events_write, &events_kernel[0]);

        q.enqueueMigrateMemObjects(ob_out, 1, &events_kernel, &events_read[0]);

        std::cout << "kernel start------" << std::endl;
        q.finish();
        std::cout << "kernel end------" << std::endl;
        res[i].diameter = max_dist_tmp[0];
        res[i].src = source_tmp[0];
        res[i].des = destination_tmp[0];
    }
}
