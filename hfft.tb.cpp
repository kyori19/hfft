#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rfft.tb.h"

int main() {
    srand(time(NULL));

    real_t real[N * MAX_LEN];
    real_t imag[N * MAX_LEN];

    for (size_t i = 0; i < N * MAX_LEN; i++) {
        real[i] = 2.0 * rand() / RAND_MAX - 1.0;
        imag[i] = 0;
    }

    hls::stream<axis_in_t> stream_in("stream_in");
    hls::stream<axis_out_t> stream_out("stream_out");

    for (size_t i = 0; i < MAX_LEN; i++) {
        axis_in_t axis_in;
        for (size_t j = 0; j < N; j++) {
            axis_in.data[j] = real[N * i + j];
        }
        axis_in.last = (i == MAX_LEN - 1);
        stream_in.write(axis_in);
    }

    for (size_t i = 0; i < MAX_LEN; i++) {
        rfft(real + N * i, imag + N * i, N);
    }

    hfft(stream_in, stream_out);

    for (size_t i = 0; i < MAX_LEN; i++) {
        axis_out_t axis_out = stream_out.read();
        for (size_t j = 0; j < N; j++) {
            if (axis_out.data[2 * j] != real[N * i + j]) {
                printf("wrong real[%d][%d]: %f != %f\n", i, j, axis_out.data[2 * j].to_float(), real[N * i + j].to_float());
                return 1;
            }
            if (axis_out.data[2 * j + 1] != imag[N * i + j]) {
                printf("wrong imag[%d][%d]: %f != %f\n", i, j, axis_out.data[2 * j + 1].to_float(), imag[N * i + j].to_float());
                return 1;
            }
        }
        if (axis_out.last != (i == MAX_LEN - 1)) {
            printf("wrong last[%d]: %d != %d\n", i, axis_out.last, i == MAX_LEN - 1);
            return 1;
        }
    }

    return 0;
}
