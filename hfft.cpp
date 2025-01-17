#include <stdlib.h>
#include "hfft.h"

void hfft_impl(real_t real[4], real_t imag[4]) {
    real_t r0 = real[0] + real[2];
    real_t r1 = real[0] - real[2];
    real_t r2 = real[1] + real[3];
    real_t r3 = real[1] - real[3];

    real[0] = r0 + r2;
    imag[0] = 0.0;
    real[1] = r1;
    imag[1] = -r3;
    real[2] = r0 - r2;
    imag[2] = 0.0;
    real[3] = r1;
    imag[3] = r3;
}

void hfft(real_t *mem_in, real_t *mem_out) {
#pragma HLS INTERFACE mode=s_axilite bundle=control port=return
#pragma HLS INTERFACE mode=m_axi port=mem_in depth=(N * MAX_LEN) offset=slave bundle=gmem_in
#pragma HLS INTERFACE mode=s_axilite bundle=control port=mem_in
#pragma HLS INTERFACE mode=m_axi port=mem_out depth=(2 * N * MAX_LEN) offset=slave bundle=gmem_out
#pragma HLS INTERFACE mode=s_axilite bundle=control port=mem_out

  handle_step:
    for (size_t step = 0; step < MAX_LEN; step++) {
        real_t real[4];
        real_t imag[4];

      load_data:
        for (size_t i = 0; i < N; i++) {
            real[i] = mem_in[step * N + i];
        }

        hfft_impl(real, imag);

      store_data:
        for (size_t i = 0; i < N; i++) {
            mem_out[step * 2 * N + 2 * i] = real[i];
            mem_out[step * 2 * N + 2 * i + 1] = imag[i];
        }
    }
}
