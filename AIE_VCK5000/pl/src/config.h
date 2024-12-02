// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


#define DWIDTH_64 64
typedef ap_axiu<DWIDTH_64, 0, 0, 0> data_64;
typedef hls::stream<data_64> axis_data_64;

#define AXI_WIDTH_A 512
typedef ap_axiu<AXI_WIDTH_A, 0, 0, 0> data_512;
typedef hls::stream<ap_uint<AXI_WIDTH_A>> axis_stream_A;

#define N 1048576
#define M 1024
#define Np 64
#define KN 1
#define Tl 2
const int LANE = AXI_WIDTH_A/DWIDTH_64;