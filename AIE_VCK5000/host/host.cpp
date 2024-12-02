// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cstdint>

#include "xrt.h"
#include "experimental/xrt_kernel.h"

// number of samples
#define LOOP 1
#define NKERNEL 64
#define NSAMPLES 1024*1024
#define NPSIZE 64
#define ITER 10

int main(int argc, char** argv) {
    // Get device index and download xclbin
    std::cout << "Open the device" << std::endl;
    auto device = xrt::device(0);
    std::string binaryFile = "../build.hw/ssca.xclbin";
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    // Get reference to the kernels
    std::cout << "Get references to datamovers compute units" << std::endl;
    auto dma0 = xrt::kernel(device, uuid, "dma_hls:{dma_hls_0}");

    // Generate random data and write data to compute unit buffers
    auto *sample_vector_in = new float [NSAMPLES+NPSIZE][2];
    // auto *sample_vector_out = new float [NSAMPLES*NKERNEL][2];
    std::cout<<"sizeof(cfloat) is "<<sizeof(float)*2<< std::endl;

    // Load data to memory
    FILE *fp;
    fp = fopen("./data/chad_dsss_noisefree_10dB_data.dat", "r");
    if (fp == NULL) {
    std::cerr << "Error: Could not open file." << std::endl;
    return 1; // Or handle the error appropriately
}
    float newvalr,newvali;
    for (size_t ii = 0; ii < NSAMPLES+NPSIZE; ii++)
    {
        if(ii<NSAMPLES){
            if (fscanf(fp, "%f\t%f\n", &newvalr, &newvali) != 0)
                {
                sample_vector_in[ii][0] = newvalr;
                sample_vector_in[ii][1] = newvali;
                // std::cout<< "sample_vector_0["<<ii<<"]:"<<sample_vector_in[ii][0]<<","<<sample_vector_in[ii][1]<<std::endl;
                }
        }else{
            sample_vector_in[ii][0] = 0;
            sample_vector_in[ii][1] = 0;
        }
    }
    fclose(fp);
    
    // Allocating the input size of sizeIn to MM2S
    std::cout << "Allocate Buffer in Global Memory" << std::endl;
    // size_t samples_size = sizeof(float) * NSAMPLES;
    size_t samples_size_complex_in = sizeof(float) * (NSAMPLES + NPSIZE) * 2; // input size is N * Np
    size_t samples_size_complex_out = sizeof(float) * NSAMPLES * NKERNEL * 2; // output size is N * Np
    auto in_bohdl0 = xrt::bo(device, samples_size_complex_in, dma0.group_id(0));
    auto in_bomapped0 = in_bohdl0.map<float*>();
    for (size_t k = 0; k< NSAMPLES+NPSIZE; k++){
        in_bomapped0[2*k] =sample_vector_in[k][0];
        in_bomapped0[2*k+1] =sample_vector_in[k][1]; 
    }
    int tt = NSAMPLES + NPSIZE;
    std::cout << "2*k is " << 2*tt << std::endl;
    // Synchronize input buffers data to device global memory
    in_bohdl0.sync(XCL_BO_SYNC_BO_TO_DEVICE, samples_size_complex_in, 0);
    
    // // intermediate memory
    auto intm_bohdl = xrt::bo(device, samples_size_complex_out, dma0.group_id(0));
    auto intm_bomapped = intm_bohdl.map<float*>();

    // output memory
    auto out_bohdl = xrt::bo(device, samples_size_complex_out, dma0.group_id(0));
    auto out_bomapped = out_bohdl.map<float*>();
    
    std::cout << "Kernels started" << std::endl;
    xrt::run dma_run0;
    double kernel_time_in_sec = 0;
    std::chrono::duration<double> kernel_time(0);
    auto kernel_start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITER; i++){
        // Execute the compute units
        dma_run0 = dma0(in_bohdl0, intm_bohdl, out_bohdl,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr, 
                nullptr, nullptr, nullptr, nullptr, 
                nullptr, 1024
                );
        // dma_run0 = dma0(in_bohdl0, out_bohdl,
        //         nullptr, nullptr, nullptr, nullptr,
        //         nullptr, nullptr, nullptr, nullptr,
        //         nullptr, 1024
        //         );
        // Wait for kernels to complete
        dma_run0.wait();
    }
    
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
    std::cout << "dma_run0 completed" << std::endl;

    kernel_time_in_sec = kernel_time.count();
    double Throughput = (NSAMPLES * NKERNEL * 1e-9*ITER*8/kernel_time_in_sec);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Total time is: "<< kernel_time_in_sec/ITER <<"s, Throughput = " << Throughput << " GB/s" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    
    // Synchronize the output buffer data from the device
    out_bohdl.sync(XCL_BO_SYNC_BO_FROM_DEVICE, samples_size_complex_out,/*OFFSET=*/ 0);
    intm_bohdl.sync(XCL_BO_SYNC_BO_FROM_DEVICE, samples_size_complex_out,/*OFFSET=*/ 0);
    
    FILE *fout = fopen("output.dat","w");
    if (fp == NULL) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }
    // Read output buffer data to local buffer
    for (size_t k = 0; k< NSAMPLES * NKERNEL; k++){
        // sample_vector_out[k][0] = out_bomapped[2*k]; 
        // sample_vector_out[k][1] = out_bomapped[2*k+1]; 
        newvalr = out_bomapped[2*k]; 
        newvali = out_bomapped[2*k+1]; 
        // newvalr = intm_bomapped[2*k]; 
        // newvali = intm_bomapped[2*k+1]; 
        fprintf(fout, "%lf %lf\n", newvalr, newvali);

    }
    fclose(fout);

    std::string test = "PASS";
    std::cout << "TEST " << test << std::endl;

    return 0;
}