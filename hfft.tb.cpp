#include <stdio.h>
#include <time.h>
#include "rfft.tb.h"

bool eq_real_approx(real_t v1, real_t v2) {
    real_t diff = v1 - v2;
    if (diff.is_neg()) {
        diff = diff.getNeg();
    }
    return diff < 0.0000001;
}

template <size_t WIDTH>
void validate_sin_cos_table() {
    for (size_t k = 0; k < WIDTH / 2; k++) {
        real_t cos_actual = sin_cos_table<WIDTH>.cos_at(k);
        real_t cos_expected = cos(-2.0 * M_PI * k / WIDTH);
        if (!eq_real_approx(cos_actual, cos_expected)) {
            printf("wrong cos_at<%lu>(%lu): %s != %s\n", WIDTH, k, cos_actual.to_string(10).c_str(), cos_expected.to_string(10).c_str());
        }

        real_t sin_actual = sin_cos_table<WIDTH>.sin_at(k);
        real_t sin_expected = sin(-2.0 * M_PI * k / WIDTH);
        if (!eq_real_approx(sin_actual, sin_expected)) {
            printf("wrong sin_at<%lu>(%lu): %s != %s\n", WIDTH, k, sin_actual.to_string(10).c_str(), sin_expected.to_string(10).c_str());
        }
    }
}

int main() {
    srand(time(NULL));

    real_t real[MAX_LEN][N];
    real_t imag[MAX_LEN][N];

    for (size_t i = 0; i < MAX_LEN; i++) {
        for (size_t j = 0; j < N; j++) {
            real[i][j] = (real_t) ((double) rand() / RAND_MAX) * 2 - 1;
            imag[i][j] = 0.0;
        }
    }

    real_t mem_out[2 * N * MAX_LEN];
    hfft((real_t *) real, mem_out);

    for (size_t i = 0; i < MAX_LEN; i++) {
        rfft(real[i], imag[i], N);
    }

    validate_sin_cos_table<8>();
    for (size_t i = 0; i < MAX_LEN; i++) {
        for (size_t j = 0; j < N; j++) {
            if (!eq_real_approx(mem_out[2 * N * i + 2 * j], real[i][j])) {
                printf("wrong real[%lu][%lu]: %s != %s\n", i, j, mem_out[2 * N * i + 2 * j].to_string(10).c_str(), real[i][j].to_string(10).c_str());
            }
            if (!eq_real_approx(mem_out[2 * N * i + 2 * j + 1], imag[i][j])) {
                printf("wrong imag[%lu][%lu]: %s != %s\n", i, j, mem_out[2 * N * i + 2 * j + 1].to_string(10).c_str(), imag[i][j].to_string(10).c_str());
            }
        }
    }

    return 0;
}
