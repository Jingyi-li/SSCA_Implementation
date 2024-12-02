#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

PLATFORMDIR  = /opt/xilinx/platforms
PF_STDNAME   = xilinx_vck5000_gen4x8_qdma_2_202220_1
XPFM = $(shell platforminfo -p $(PF_STDNAME) --json="file")
TARGET := hw

# OUTPUT PRODUCTS 
BUILD_DIR = build.$(TARGET)
SRC_DIR = $(shell readlink -f ./)

INCLUDES           = --include=$(SRC_DIR)/ --include=$(SRC_DIR)/inc --include=$(SRC_DIR)/kernels
TEST_BENCH         = $(SRC_DIR)/test_ssrfft_1m.cpp
OTHER_OPTS         = --aie.constraints=./shim_loc.json --aie.xlopt=1
ROUTER_OPTS        = --aie.Xmapper=disableFloorplanning --aie.Xrouter=disablepathbalancing \
	             --aie.Xrouter=disableSSFifoSharing --aie.disable-multirate
# NUM_ITERATION      = 8
NUM_ITERATION      = 1
TEMP_FILE          = t.txt
LOG_FILE           = log.txt

SOURCES            = $(SRC_DIR)/inc/*.h $(SRC_DIR)/kernels/*.cpp $(SRC_DIR)/kernels/*.h $(SRC_DIR)/test_ssrfft_1m.h $(TEST_BENCH)

.PHONY:	all testvectors aie_compile aie_sim aie_check_op aie_get_tp x86_compile x86_sim x86_check_op clean cleanall

all:	$(BUILD_DIR)/libadf.a

# generate test vectors for AIE simulation
testvectors:
	cd ../matlab && matlab -nodisplay -nojvm -r "test_ssrfft_1mpt($(NUM_ITERATION),0,0);quit"

$(BUILD_DIR)/libadf.a:	$(SOURCES)
	@mkdir -p $(BUILD_DIR);
	cd $(BUILD_DIR); \
	echo "#define NUM_ITERATION (1 * $(NUM_ITERATION))" > ../test_cfg.h ;\
	awk -f $(SRC_DIR)/gen_shim_constrain.awk $(SRC_DIR)/shim_loc.cfg > ./shim_loc.json ;\
	v++ --compile --mode aie --target=hw --aie.verbose $(INCLUDES) $(TEST_BENCH) $(OTHER_OPTS) $(ROUTER_OPTS) --platform=$(XPFM) 2>&1 | tee -a $(LOG_FILE)

aie_sim:
	cd $(BUILD_DIR); \
	@rm -rf aiesimulator_output ;\
	aiesimulator --pkg-dir=Work --i=.. --profile --dump-vcd=foo ; 
# aiesimulator --pkg-dir=Work --i=.. --profile --dump-vcd=foo ; 

clean:
	/bin/rm -rf $(BUILD_DIR)
	/bin/rm -rf Work* *.log *.xpe xnwOut .X* .A*
	/bin/rm -rf aiesimulator_output tmp pl_sample* *.a test_cfg.h
	/bin/rm -rf $(TEMP_FILE) log* $(LOG_FILE) ISS_RPC* Map_*.csv sol.db
	/bin/rm -rf *.jou *.debug .crashReporter temp shim_loc.json
	/bin/rm -rf hw x86simulator_output plio_throughput_info.json
	/bin/rm -rf ip* .nf*
	/bin/rm -rf xnwOut vivado_* vitis_*
	/bin/rm -rf *.a .Xil *.xo *.xpe *.xdc vivado*
	/bin/rm -rf *.log *.pdi  pmc* *.cdo ./host/*.elf ./host/*.o
	/bin/rm -rf host/test_cfg.h sol.db Map_Report.csv

cleanall:	clean
	/bin/rm -rf data

