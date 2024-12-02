# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import argparse
import numpy as np
import pyxrt
import time

import ssca_module

SEED = 32
flag_res = False
flag_print = False

def read_file(file: str, plio: int=128):
    data = []  
    with open(file, 'r') as f:
        for line in f:
            values = line.strip().split(" ")
            data.extend([float(val) for val in values if val])
    return np.array(data, dtype=np.float32)

def main(xclbin: str = None, idx: int = 0, size: int = 128):
    """Run ssca on the VCK5000"""
    device = pyxrt.device(idx)
    xbin = pyxrt.xclbin(xclbin)
    uuid = device.load_xclbin(xbin)

    # Channel Data Product
    mm2s_32    = pyxrt.kernel(device, uuid, "mm2s_32:{mm2s_32_ssca}")
    s2mm_32    = pyxrt.kernel(device, uuid, "s2mm_32:{s2mm_32_ssca}")
    mm2s_32_v2 = pyxrt.kernel(device, uuid, "mm2s_32:{mm2s_32_ssca_v2}")
    s2mm_32_v2 = pyxrt.kernel(device, uuid, "s2mm_32:{s2mm_32_ssca_v2}")
    mm2s_32_v4 = pyxrt.kernel(device, uuid, "mm2s_32:{mm2s_32_ssca_v4}")
    s2mm_32_v4 = pyxrt.kernel(device, uuid, "s2mm_32:{s2mm_32_ssca_v4}")
    mm2s_32_v8 = pyxrt.kernel(device, uuid, "mm2s_32:{mm2s_32_ssca_v8}")
    s2mm_32_v8 = pyxrt.kernel(device, uuid, "s2mm_32:{s2mm_32_ssca_v8}")

    mm2s_64    = pyxrt.kernel(device, uuid, "mm2s_64:{mm2s_64_ssca}")
    s2mm_64    = pyxrt.kernel(device, uuid, "s2mm_64:{s2mm_64_ssca}")
    mm2s_64_v2 = pyxrt.kernel(device, uuid, "mm2s_64:{mm2s_64_ssca_v2}")
    s2mm_64_v2 = pyxrt.kernel(device, uuid, "s2mm_64:{s2mm_64_ssca_v2}")
    mm2s_64_v4 = pyxrt.kernel(device, uuid, "mm2s_64:{mm2s_64_ssca_v4}")
    s2mm_64_v4 = pyxrt.kernel(device, uuid, "s2mm_64:{s2mm_64_ssca_v4}")
    mm2s_64_v8 = pyxrt.kernel(device, uuid, "mm2s_64:{mm2s_64_ssca_v8}")
    s2mm_64_v8 = pyxrt.kernel(device, uuid, "s2mm_64:{s2mm_64_ssca_v8}")

    mm2s_128    = pyxrt.kernel(device, uuid, "mm2s_128:{mm2s_128_ssca}")
    s2mm_128    = pyxrt.kernel(device, uuid, "s2mm_128:{s2mm_128_ssca}")
    mm2s_128_v2 = pyxrt.kernel(device, uuid, "mm2s_128:{mm2s_128_ssca_v2}")
    s2mm_128_v2 = pyxrt.kernel(device, uuid, "s2mm_128:{s2mm_128_ssca_v2}")
    mm2s_128_v4 = pyxrt.kernel(device, uuid, "mm2s_128:{mm2s_128_ssca_v4}")
    s2mm_128_v4 = pyxrt.kernel(device, uuid, "s2mm_128:{s2mm_128_ssca_v4}")
    mm2s_128_v8 = pyxrt.kernel(device, uuid, "mm2s_128:{mm2s_128_ssca_v8}")
    s2mm_128_v8 = pyxrt.kernel(device, uuid, "s2mm_128:{s2mm_128_ssca_v8}")

    # shift register buffer and size
    input_size = size
    buf_byte_size  = np.dtype(np.complex64).itemsize * input_size

    print("Allocate and initialize buffers")
    in_buf_mm2s_32     = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_32.group_id(0))
    out_buf_s2mm_32    = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_32.group_id(0))
    in_buf_mm2s_32_v2  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_32_v2.group_id(0))
    out_buf_s2mm_32_v2 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_32_v2.group_id(0))
    in_buf_mm2s_32_v4  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_32_v4.group_id(0))
    out_buf_s2mm_32_v4 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_32_v4.group_id(0))
    in_buf_mm2s_32_v8  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_32_v8.group_id(0))
    out_buf_s2mm_32_v8 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_32_v8.group_id(0))

    in_buf_mm2s_64     = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_64.group_id(0))
    out_buf_s2mm_64    = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_64.group_id(0))
    in_buf_mm2s_64_v2  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_64_v2.group_id(0))
    out_buf_s2mm_64_v2 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_64_v2.group_id(0))
    in_buf_mm2s_64_v4  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_64_v4.group_id(0))
    out_buf_s2mm_64_v4 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_64_v4.group_id(0))
    in_buf_mm2s_64_v8  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_64_v8.group_id(0))
    out_buf_s2mm_64_v8 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_64_v8.group_id(0))

    in_buf_mm2s_128     = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_128.group_id(0))
    out_buf_s2mm_128    = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_128.group_id(0))
    in_buf_mm2s_128_v2  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_128_v2.group_id(0))
    out_buf_s2mm_128_v2 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_128_v2.group_id(0))
    in_buf_mm2s_128_v4  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_128_v4.group_id(0))
    out_buf_s2mm_128_v4 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_128_v4.group_id(0))
    in_buf_mm2s_128_v8  = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, mm2s_128_v8.group_id(0))
    out_buf_s2mm_128_v8 = pyxrt.bo(device, buf_byte_size, pyxrt.bo.normal, s2mm_128_v8.group_id(0))

    print("Generate Random Value for input")
    np.random.seed(SEED)
    sizef = size*2
    in_data = np.random.uniform(-1, 1, size=sizef).astype(np.float32)
    

    print(f"Write to buffer")
    in_buf_mm2s_32.write(in_data, 0)
    in_buf_mm2s_32_v2.write(in_data, 0) 
    in_buf_mm2s_32_v4.write(in_data, 0) 
    in_buf_mm2s_32_v8.write(in_data, 0) 
    
    in_buf_mm2s_64.write(in_data, 0)
    in_buf_mm2s_64_v2.write(in_data, 0) 
    in_buf_mm2s_64_v4.write(in_data, 0) 
    in_buf_mm2s_64_v8.write(in_data, 0) 

    in_buf_mm2s_128.write(in_data, 0)
    in_buf_mm2s_128_v2.write(in_data, 0) 
    in_buf_mm2s_128_v4.write(in_data, 0) 
    in_buf_mm2s_128_v8.write(in_data, 0) 
    


    print(f"Move vectors from host memory to VCK5000 memory")
    in_buf_mm2s_32.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_32_v2.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_32_v4.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_32_v8.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)

    in_buf_mm2s_64.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_64_v2.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_64_v4.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_64_v8.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)

    in_buf_mm2s_128.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_128_v2.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_128_v4.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)
    in_buf_mm2s_128_v8.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, buf_byte_size, 0)

    if flag_print and l % 64 ==0: print(f"Run CDP on the VCK5000 in loops {l}")
    run_mm2s_32     = mm2s_32(in_buf_mm2s_32,  None, buf_byte_size)
    run_s2mm_32     = s2mm_32(out_buf_s2mm_32, None, buf_byte_size)
    run_mm2s_32_v2  = mm2s_32_v2(in_buf_mm2s_32_v2,  None, buf_byte_size)
    run_s2mm_32_v2  = s2mm_32_v2(out_buf_s2mm_32_v2, None, buf_byte_size)
    run_mm2s_32_v4  = mm2s_32_v4(in_buf_mm2s_32_v4,  None, buf_byte_size)
    run_s2mm_32_v4  = s2mm_32_v4(out_buf_s2mm_32_v4, None, buf_byte_size)
    run_mm2s_32_v8  = mm2s_32_v8(in_buf_mm2s_32_v8,  None, buf_byte_size)
    run_s2mm_32_v8  = s2mm_32_v8(out_buf_s2mm_32_v8, None, buf_byte_size)

    run_mm2s_64     = mm2s_64(in_buf_mm2s_64,  None, buf_byte_size)
    run_s2mm_64     = s2mm_64(out_buf_s2mm_64, None, buf_byte_size)
    run_mm2s_64_v2  = mm2s_64_v2(in_buf_mm2s_64_v2,  None, buf_byte_size)
    run_s2mm_64_v2  = s2mm_64_v2(out_buf_s2mm_64_v2, None, buf_byte_size)
    run_mm2s_64_v4  = mm2s_64_v4(in_buf_mm2s_64_v4,  None, buf_byte_size)
    run_s2mm_64_v4  = s2mm_64_v4(out_buf_s2mm_64_v4, None, buf_byte_size)
    run_mm2s_64_v8  = mm2s_64_v8(in_buf_mm2s_64_v8,  None, buf_byte_size)
    run_s2mm_64_v8  = s2mm_64_v8(out_buf_s2mm_64_v8, None, buf_byte_size)

    run_mm2s_128     = mm2s_128(in_buf_mm2s_128,  None, buf_byte_size)
    run_s2mm_128     = s2mm_128(out_buf_s2mm_128, None, buf_byte_size)
    run_mm2s_128_v2  = mm2s_128_v2(in_buf_mm2s_128_v2,  None, buf_byte_size)
    run_s2mm_128_v2  = s2mm_128_v2(out_buf_s2mm_128_v2, None, buf_byte_size)
    run_mm2s_128_v4  = mm2s_128_v4(in_buf_mm2s_128_v4,  None, buf_byte_size)
    run_s2mm_128_v4  = s2mm_128_v4(out_buf_s2mm_128_v4, None, buf_byte_size)
    run_mm2s_128_v8  = mm2s_128_v8(in_buf_mm2s_128_v8,  None, buf_byte_size)
    run_s2mm_128_v8  = s2mm_128_v8(out_buf_s2mm_128_v8, None, buf_byte_size)

    
    run_mm2s_32.wait()
    run_s2mm_32.wait()
    run_mm2s_32_v2.wait()
    run_s2mm_32_v2.wait()
    run_mm2s_32_v4.wait()
    run_s2mm_32_v4.wait()
    run_mm2s_32_v8.wait()
    run_s2mm_32_v8.wait()

    run_mm2s_64.wait()
    run_s2mm_64.wait()
    run_mm2s_64_v2.wait()
    run_s2mm_64_v2.wait()
    run_mm2s_64_v4.wait()
    run_s2mm_64_v4.wait()
    run_mm2s_64_v8.wait()
    run_s2mm_64_v8.wait()

    run_mm2s_128.wait()
    run_s2mm_128.wait()
    run_mm2s_128_v2.wait()
    run_s2mm_128_v2.wait()
    run_mm2s_128_v4.wait()
    run_s2mm_128_v4.wait()
    run_mm2s_128_v8.wait()
    run_s2mm_128_v8.wait()
    
    cdp_outs = []
    cdp_out = np.zeros(input_size, dtype=np.complex64) #  for only one kernels output
    print(f"Move vectors from VCK5000 memory to host memory")
    out_buf_s2mm_32.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_32.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_32_v2.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_32_v2.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_32_v4.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_32_v4.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_32_v8.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_32_v8.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)

    out_buf_s2mm_64.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_64.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_64_v2.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_64_v2.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_64_v4.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_64_v4.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_64_v8.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_64_v8.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)

    out_buf_s2mm_128.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_128.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_128_v2.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_128_v2.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_128_v4.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_128_v4.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)
    out_buf_s2mm_128_v8.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                buf_byte_size, 0)
    cdp_out = np.frombuffer(out_buf_s2mm_128_v8.read(buf_byte_size, 0), dtype=np.complex64)
    cdp_outs.append(cdp_out)

    cdp_outs = np.array(cdp_outs)
    print(cdp_outs.shape)
    for i in range(12):
        print(f"current i is {i}")
        print(cdp_outs[i][0:20])

    print(f"Finish run CDP_output")
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Run vector addition on VCK5000")
    parser.add_argument("--xclbin", help="XCLBIN files", required=True)
    parser.add_argument("--size", type=int, default=2**20,
                        help="Number of elements", required=False)
    parser.add_argument("--device", type=int, default=0,
                        help="Device Index", required=False)
    args = parser.parse_args()

    main(args.xclbin, args.device, args.size)
