# ssca_module.py

import numpy as np
import math


PI = 3.14159265358979323846
WINDOW_SIZE = 64
WINDOW_SIZE_LOG2 = int(np.log2(WINDOW_SIZE))
WINDOW_SIZE_HALF = WINDOW_SIZE // 2
radian_step = 2 * PI * 1.0 / WINDOW_SIZE

# Chebyshev window function coefficient for length 128
CHEBYSHEV_FLOAT_128_TAPS = np.array([
    0.000423,0.000494,0.000774,0.001155,0.001658,0.002310,0.003141,0.004184,
    0.005476,0.007058,0.008973,0.011270,0.014000,0.017216,0.020975,0.025337,
    0.030363,0.036115,0.042657,0.050053,0.058364,0.067653,0.077977,0.089393,
    0.101953,0.115701,0.130678,0.146917,0.164443,0.183270,0.203405,0.224843,
    0.247567,0.271548,0.296746,0.323107,0.350563,0.379035,0.408428,0.438637,
    0.469543,0.501015,0.532910,0.565077,0.597354,0.629571,0.661553,0.693118,
    0.724082,0.754256,0.783456,0.811494,0.838190,0.863368,0.886857,0.908497,
    0.928139,0.945645,0.960891,0.973768,0.984184,0.992063,0.997348,1.000000,
    1.000000,0.997348,0.992063,0.984184,0.973768,0.960891,0.945645,0.928139,
    0.908497,0.886857,0.863368,0.838190,0.811494,0.783456,0.754256,0.724082,
    0.693118,0.661553,0.629571,0.597354,0.565077,0.532910,0.501015,0.469543,
    0.438637,0.408428,0.379035,0.350563,0.323107,0.296746,0.271548,0.247567,
    0.224843,0.203405,0.183270,0.164443,0.146917,0.130678,0.115701,0.101953,
    0.089393,0.077977,0.067653,0.058364,0.050053,0.042657,0.036115,0.030363,
    0.025337,0.020975,0.017216,0.014000,0.011270,0.008973,0.007058,0.005476,
    0.004184,0.003141,0.002310,0.001658,0.001155,0.000774,0.000494,0.000423
], dtype=np.float32)

CHEBYSHEV_FLOAT_64_TAPS = np.array([
    0.000381, 0.000878, 0.001843, 0.003448, 0.005956, 0.009691, 0.015033, 0.022414,
    0.032309, 0.045220, 0.061659, 0.082120, 0.107054, 0.136837, 0.171738, 0.211889,
    0.257257, 0.307621, 0.362554, 0.421416, 0.483358, 0.547330, 0.612112, 0.676339,
    0.738553, 0.797250, 0.850935, 0.898187, 0.937711, 0.968395, 0.989361, 1.000000,
    1.000000, 0.989361, 0.968395, 0.937711, 0.898187, 0.850935, 0.797250, 0.738553,
    0.676339, 0.612112, 0.547330, 0.483358, 0.421416, 0.362554, 0.307621, 0.257257,
    0.211889, 0.171738, 0.136837, 0.107054, 0.082120, 0.061659, 0.045220, 0.032309,
    0.022414, 0.015033, 0.009691, 0.005956, 0.003448, 0.001843, 0.000878, 0.000381
], dtype=np.float32)

CHEBYSHEV_FLOAT_32_TAPS = np.array([
    0.000616,0.002682,0.007949,0.019017,0.039328,0.072837,0.123387,0.193835,
    0.285089,0.395264,0.519208,0.648591,0.772664,0.879611,0.958285,1.000000,
    1.000000,0.958285,0.879611,0.772664,0.648591,0.519208,0.395264,0.285089,
    0.193835,0.123387,0.072837,0.039328,0.019017,0.007949,0.002682,0.000616
], dtype=np.float32)

CHEBYSHEV_FLOAT_8_TAPS = np.array([
    0.036384,0.225355,0.624160,1.000000,1.000000,0.624160,0.225355,0.036384
], dtype=np.float32)

if WINDOW_SIZE == 64: 
    chebyshev_float_taps = CHEBYSHEV_FLOAT_64_TAPS
elif WINDOW_SIZE == 128:
    chebyshev_float_taps = CHEBYSHEV_FLOAT_128_TAPS
elif WINDOW_SIZE == 32:
    chebyshev_float_taps = CHEBYSHEV_FLOAT_32_TAPS
elif WINDOW_SIZE == 8:
    chebyshev_float_taps = CHEBYSHEV_FLOAT_8_TAPS
else:
    raise ValueError("Invalid WINDOW_SIZE")

### functions ###
def bitrevorder(x):
    n = len(x)
    j = 0

    # Rearrange the array elements in bit-reversed order
    for i in range(1, n):
        bit = n >> 1
        while j >= bit:
            j -= bit
            bit >>= 1
        j += bit

        # Swap elements
        if i < j:
            x[i], x[j] = x[j], x[i]

    return x
def get_fft_coef(WINDOW_SIZE):
    fft_coef = np.zeros(WINDOW_SIZE // 2, dtype=np.complex64)
    data = np.loadtxt('../aie/data/fft_coef.dat', delimiter=',')
    fft_coef = data[:,0].astype(np.float32)+1j*data[:,1].astype(np.float32)
    return fft_coef

def get_exp_coef(window_id):
    exp_coef = np.zeros(WINDOW_SIZE, dtype=np.complex64)
    data = np.loadtxt(f"../aie/data/exp_coef_{window_id}.dat", delimiter=',')
    exp_coef = data[:,0].astype(np.float32)+1j*data[:,1].astype(np.float32)
    return exp_coef

def process_func(fft_in):
    fft_coef = get_fft_coef(WINDOW_SIZE)
    bitrevorder(fft_in)
    # Initialize FFT buffer
    FFT_buffer = np.zeros((WINDOW_SIZE_LOG2, WINDOW_SIZE), dtype=np.complex64)

    # Stage 1
    for i in range(0, WINDOW_SIZE, 2):
        FFT_buffer[0][i] = fft_in[i] + fft_in[i+1]
        FFT_buffer[0][i+1] = fft_in[i] - fft_in[i+1]

    halfsize = 2

    # Stage 2
    for i in range(0, WINDOW_SIZE, 4):
        for j in range(i, i + halfsize):
            if j == i:
                FFT_buffer[1][j+halfsize] = FFT_buffer[0][j] - FFT_buffer[0][j+halfsize]
                FFT_buffer[1][j] = FFT_buffer[0][j] + FFT_buffer[0][j+halfsize]
            else:
                FFT_buffer[1][j+halfsize] = (FFT_buffer[0][j].real - FFT_buffer[0][j+halfsize].imag) + 1j *(FFT_buffer[0][j].imag + FFT_buffer[0][j+halfsize].real) 

                FFT_buffer[1][j] = (FFT_buffer[0][j].real + FFT_buffer[0][j+halfsize].imag) + 1j*(FFT_buffer[0][j].imag - FFT_buffer[0][j+halfsize].real) 

    # Rest stages
    for idxs in range(2, WINDOW_SIZE_LOG2):
        halfsize *= 2
        tablestep = WINDOW_SIZE // 2 // halfsize
        for i in range(0, WINDOW_SIZE, halfsize*2):
            k = 0
            for j in range(i, i + halfsize):
                temp = fft_coef[k] * FFT_buffer[idxs-1][j+halfsize]
                FFT_buffer[idxs][j+halfsize] = FFT_buffer[idxs-1][j] - temp
                FFT_buffer[idxs][j] = FFT_buffer[idxs-1][j] + temp
                k += tablestep

    # FFT shift
    fft_out = np.zeros(WINDOW_SIZE, dtype=np.complex64)
    for i in range(WINDOW_SIZE_HALF):
        fft_out[i+WINDOW_SIZE_HALF] = FFT_buffer[WINDOW_SIZE_LOG2-1][i]
        fft_out[i] = FFT_buffer[WINDOW_SIZE_LOG2-1][i+WINDOW_SIZE_HALF]

    return fft_out

def ssca_func(input_data):
    size = WINDOW_SIZE
    input_len = len(input_data)//2
    # use the end Np-1 items to fill the first Np-1 items which is assumed to be saved in register
    input0 = np.concatenate([input_data[(len(input_data)-2*(size-1)):len(input_data)],input_data])
    downconversion_c = np.zeros(size, dtype = complex)
    cdp_c_loops = []

    for i in range(input_len):
        window_id = i
        a_real = input0[window_id*2: (window_id+WINDOW_SIZE)*2: 2]
        a_imag = input0[window_id*2+1: (window_id+WINDOW_SIZE)*2: 2]
        a_complex = a_real.astype(np.float32) + 1j * a_imag.astype(np.float32)
        c_complex = a_complex * chebyshev_float_taps
        # firstfft_complex = np.fft.fft(c_complex)
        # firstfftshift_c = np.fft.fftshift(firstfft_complex)
        firstfftshift_c = process_func(c_complex)

        down_conversion_coeff = get_exp_coef(window_id%WINDOW_SIZE)
        for i in range(size):
            downconversion_c[i] =  firstfftshift_c[i] * down_conversion_coeff[i]
        
        # for i in range(size):
        #     dc_idx = (window_id * i) % WINDOW_SIZE
        #     downconversion_c[i] =  firstfftshift_c[i] * down_conversion_coeff[dc_idx]
        cdp_c = downconversion_c * a_complex[WINDOW_SIZE//2].conjugate()
        cdp_c_loops.append(cdp_c)
    cdp_c_loops = np.array(cdp_c_loops).reshape(-1)
    return cdp_c_loops

def ssca_func2(input_data,add_data):
    size = WINDOW_SIZE
    input_len = len(input_data)//2
    # use the end Np-1 items to fill the first Np-1 items which is assumed to be saved in register
    input0 = np.concatenate([add_data,input_data])
    downconversion_c = np.zeros(size, dtype = complex)
    cdp_c_loops = []

    for i in range(input_len):
        window_id = i
        a_real = input0[window_id*2: (window_id+WINDOW_SIZE)*2: 2]
        a_imag = input0[window_id*2+1: (window_id+WINDOW_SIZE)*2: 2]
        a_complex = a_real.astype(np.float32) + 1j * a_imag.astype(np.float32)
        c_complex = a_complex * chebyshev_float_taps
        # firstfft_complex = np.fft.fft(c_complex)
        # firstfftshift_c = np.fft.fftshift(firstfft_complex)
        firstfftshift_c = process_func(c_complex)

        down_conversion_coeff = get_exp_coef(window_id%WINDOW_SIZE)
        for i in range(size):
            downconversion_c[i] =  firstfftshift_c[i] * down_conversion_coeff[i]
        
        # for i in range(size):
        #     dc_idx = (window_id * i) % WINDOW_SIZE
        #     downconversion_c[i] =  firstfftshift_c[i] * down_conversion_coeff[dc_idx]
        cdp_c = downconversion_c * a_complex[WINDOW_SIZE//2].conjugate()
        cdp_c_loops.append(cdp_c)
    cdp_c_loops = np.array(cdp_c_loops).reshape(-1)
    return cdp_c_loops


def second_fft(input_data,input_len,kernel_num):
    input_ffts = []
    for i in range(kernel_num):
        a_real = input_data[0+i*input_len*2: (i+1)*input_len*2: 2]
        a_imag = input_data[1+i*input_len*2: (i+1)*input_len*2: 2]
        input_fft = a_real.astype(np.float32) + 1j * a_imag.astype(np.float32)
        input_ffts.append(np.fft.fft(input_fft))
    return np.concatenate(input_ffts)

def second_fft_res(input_data,input_len,kernel_num):
    input_ffts = []
    for i in range(kernel_num):
        a_real = input_data[0+i*input_len*2: (i+1)*input_len*2: 2]
        a_imag = input_data[1+i*input_len*2: (i+1)*input_len*2: 2]
        input_fft = a_real.astype(np.float32) + 1j * a_imag.astype(np.float32)
        input_ffts.append(np.fft.fft(input_fft))
    return np.concatenate(input_ffts)