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

void hfft(hls::stream<axis_in_t> &stream_in, hls::stream<axis_out_t> &stream_out) {
#pragma HLS INTERFACE mode=s_axilite bundle=control port=return
#pragma HLS INTERFACE mode=axis port=stream_in
#pragma HLS INTERFACE mode=axis port=stream_out

  handle_stream:
    for (size_t idx = 0; idx < MAX_LEN; idx++) {
        real_t real[4];
        real_t imag[4];

        axis_in_t axis_in = stream_in.read();
        axis_out_t axis_out;

      load_data:
        for (size_t i = 0; i < N; i++) {
            real[i] = axis_in.data[i];
        }

        hfft_impl(real, imag);

      store_data:
        for (size_t i = 0; i < N; i++) {
            axis_out.data[2 * i] = real[i];
            axis_out.data[2 * i + 1] = imag[i];
        }

        axis_out.keep = -1;
        axis_out.strb = -1;
        axis_out.last = idx == MAX_LEN - 1;
        stream_out.write(axis_out);
    }
}
