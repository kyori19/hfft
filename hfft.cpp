#include "hfft.h"

/**
 * WARNING: To use this code, you need to install multiple versions of Vitis HLS.
 *  - Use Vitis HLS 2022 for C Simulation
 *  - Use Vitis HLS 2024 for C Synthesis
 */

template <size_t WIDTH>
void hfft_impl(real_t real[WIDTH], real_t imag[WIDTH]) {
    real_t even_real[WIDTH / 2];
    real_t even_imag[WIDTH / 2];
    real_t odd_real[WIDTH / 2];
    real_t odd_imag[WIDTH / 2];

  divide_input:
    for (size_t i = 0; i < WIDTH / 2; i++) {
        even_real[i] = real[2 * i];
        even_imag[i] = imag[2 * i];
        odd_real[i] = real[2 * i + 1];
        odd_imag[i] = imag[2 * i + 1];
    }

    hfft_impl<WIDTH / 2>(even_real, even_imag);
    hfft_impl<WIDTH / 2>(odd_real, odd_imag);

  merge_output:
    for (size_t i = 0; i < WIDTH / 2; i++) {
        real_t t_real = sin_cos_table<WIDTH>.cos_at(i) * odd_real[i] - sin_cos_table<WIDTH>.sin_at(i) * odd_imag[i];
        real_t t_imag = sin_cos_table<WIDTH>.sin_at(i) * odd_real[i] + sin_cos_table<WIDTH>.cos_at(i) * odd_imag[i];

        real[i] = even_real[i] + t_real;
        imag[i] = even_imag[i] + t_imag;
        real[i + WIDTH / 2] = even_real[i] - t_real;
        imag[i + WIDTH / 2] = even_imag[i] - t_imag;
    }
}

template <>
void hfft_impl<4>(real_t real[4], real_t imag[4]) {
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
        real_t real[N];
        real_t imag[N];

      load_data:
        for (size_t i = 0; i < N; i++) {
            real[i] = mem_in[step * N + i];
            imag[i] = 0.0;
        }

        hfft_impl<N>(real, imag);

      store_data:
        for (size_t i = 0; i < N; i++) {
            mem_out[step * 2 * N + 2 * i] = real[i];
            mem_out[step * 2 * N + 2 * i + 1] = imag[i];
        }
    }
}
