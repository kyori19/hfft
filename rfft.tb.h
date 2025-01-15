#include <math.h>
#include <stdlib.h>
#include "hfft.h"

void rfft4(real_t *real, real_t *imag) {
    real_t r0 = real[0] + real[2];
    real_t r1 = real[0] - real[2];
    real_t r2 = real[1] + real[3];
    real_t r3 = real[1] - real[3];

    real[0] = r0 + r2;
    real[1] = r1;
    imag[1] = -r3;
    real[2] = r0 - r2;
    real[3] = r1;
    imag[3] = r3;
}

void rfft(real_t *real, real_t *imag, int n) {
    if (n == 4) {
        rfft4(real, imag);
        return;
    }

    real_t *even_real = (real_t *) malloc(n / 2 * sizeof(real_t));
    real_t *even_imag = (real_t *) malloc(n / 2 * sizeof(real_t));
    real_t *odd_real = (real_t *) malloc(n / 2 * sizeof(real_t));
    real_t *odd_imag = (real_t *) malloc(n / 2 * sizeof(real_t));

    for (int i = 0; i < n / 2; i++) {
        even_real[i] = real[2 * i];
        even_imag[i] = imag[2 * i];
        odd_real[i] = real[2 * i + 1];
        odd_imag[i] = imag[2 * i + 1];
    }

    rfft(even_real, even_imag, n / 2);
    rfft(odd_real, odd_imag, n / 2);

    for (int k = 0; k < n / 2; k++) {
        real_t t_real = (real_t) cos(-2.0 * M_PI * k / n) * odd_real[k] - (real_t) sin(-2.0 * M_PI * k / n) * odd_imag[k];
        real_t t_imag = (real_t) sin(-2.0 * M_PI * k / n) * odd_real[k] + (real_t) cos(-2.0 * M_PI * k / n) * odd_imag[k];

        real[k] = even_real[k] + t_real;
        imag[k] = even_imag[k] + t_imag;
        real[k + n / 2] = even_real[k] - t_real;
        imag[k + n / 2] = even_imag[k] - t_imag;
    }

    free(even_real);
    free(even_imag);
    free(odd_real);
    free(odd_imag);
}
