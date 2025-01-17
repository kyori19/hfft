#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rfft.tb.h"

int main() {
    srand(time(NULL));

    real_t real[MAX_LEN][N];
    real_t imag[MAX_LEN][N];

    for (size_t i = 0; i < MAX_LEN; i++) {
        for (size_t j = 0; j < N; j++) {
            real[i][j] = (real_t) rand() / RAND_MAX;
            imag[i][j] = 0.0;
        }
    }

    real_t mem_out[2 * N * MAX_LEN];

    for (size_t i = 0; i < MAX_LEN; i++) {
        rfft(real[i], imag[i], N);
    }

    hfft((real_t *) real, mem_out);

    for (size_t i = 0; i < MAX_LEN; i++) {
        for (size_t j = 0; j < N; j++) {
            if (abs((float) (mem_out[2 * N * i + 2 * j] - real[i][j])) > 1e-4) {
                printf("wrong real[%d][%d]: %f != %f\n", i, j, mem_out[2 * N * i + 2 * j].to_float(), real[i][j].to_float());
                return 1;
            }
            if (abs((float) (mem_out[2 * N * i + 2 * j + 1] - imag[i][j])) > 1e-4) {
                printf("wrong imag[%d][%d]: %f != %f\n", i, j, mem_out[2 * N * i + 2 * j + 1].to_float(), imag[i][j].to_float());
                return 1;
            }
        }
    }

    return 0;
}
